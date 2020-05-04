import math
import re
import sys
import numpy as np
from pathlib import Path
from typing import Any, Counter, Dict, List, Optional, Set, Tuple, Union
from scipy.special import logsumexp
from collections import Counter, defaultdict
from Prob import Prob


class Tagger:

    def __init__(self):
        #these will be dictionaries - key by tuple s.t. if P(H|start) --> (H, start) is the key and we store the prob as a values
        self.emiss_prob = dict()
        self.trans_prob = dict()
        self.initial_prob = dict()

        #these are just the set of the possible observation or tags
        self.tag_vocab = set()
        self.observation_vocab = set()

        # Dictionary of possible tags for each unique observation
        self.possible_tags = dict()

        # Counter objects used to compute probabilities from the training data
        self.tag_trans = Counter() # counts the number of times we go from 1 state to another
        self.tag_tot = Counter() # counts the number of times each tag appears
        self.tag_to_ob = Counter() # counts the number of times each tag appears with a certain observation
        self.obs_tot = Counter() #counts number of times each obs ppears

    # Read and store a training file
    def read_train_file(self, file_name):
        tags = []
        obs = []
        with open(file_name, "r") as f:
            for line in f:
                line = line.strip().split("/")
                observation = line[0]
                tag = line[1]

                # Construct vocabularies
                self.tag_vocab.add(tag)
                self.observation_vocab.add(observation)

                # Construct dictionary of possible tags for each observation
                if observation not in self.possible_tags.keys():
                    self.possible_tags[observation] = set()
                self.possible_tags[observation].add(tag)

                # Store current sequence
                tags.append(tag)
                obs.append(observation)

        self.possible_tags["OOV"] = set([tag for tag in list(self.tag_vocab) if tag != '###'])
        self.observation_vocab.add("OOV")
        return tags, obs

    def read_test_file(self, file_name): #MAKE SURE TO ADD OOV IF OBS NOT IN OBS VOCAB
        tags = []
        observations = []
        with open(file_name, "r") as f:
            for line in f:
                line = line.strip().split("/")
                obs = line[0]
                tag = line[1]
                tags.append(tag)
                if obs not in self.observation_vocab:
                    observations.append("OOV")
                else:
                    observations.append(obs)
        return tags, observations

    def counting(self, tags, obs):
        # Count appearances of tag bigrams (sliding window of size 2 over tags)
        for i in range(len(tags) - 1):
            self.tag_trans[(tags[i], tags[i + 1])] += 1

        # Count appearances of tag unigrams - excluding the very first ### (per G.5)
        for i in range(1, len(tags)):
            self.tag_tot[tags[i]] += 1
        for i in range(1, len(obs)):
            self.obs_tot[obs[i]] += 1
        # Count appeareances of tag-observation pairs
        for i in range(len(obs)):
            self.tag_to_ob[(tags[i], obs[i])] += 1

        self.word_amount_tags = len(tags) - 1
        self.word_amount_obs = len(obs) - 1

    # Compute transition and emission probability dictionaries for current training file using add-lambda smoothing
    def add_lambda(self, lam):

        # Compute transition probabilities
        for key in self.tag_trans.keys():
            #try:
            p = (self.tag_trans[key] + lam) / (self.tag_tot[key[0]] + (lam * len(self.tag_vocab)))
            # Update probability dictionaries
            self.trans_prob[(key[1], key[0])] = Prob(p)
            if key[0] == "###":
                 self.initial_prob[key[1]] = Prob(p)

        # Compute emission probabilities
        for key in self.tag_to_ob.keys():
            #try:
            p = (self.tag_to_ob[key] + lam) / (self.tag_tot[key[0]] + (lam * len(self.observation_vocab)))
            # Update probability dictionary
            self.emiss_prob[(key[1], key[0])] = Prob(p)

    def backoff_add_lambda(self, lam):


        for key in self.tag_trans.keys():
            #try:
            #p_z = (self.tag_tot[key[1]] + lam)/ (self.word_amount_tags + lam * len(self.tag_vocab))
            p_z = (self.tag_tot[key[1]] + lam)/ (self.tag_tot[key[1]] + lam * len(self.tag_vocab))
            #p_z = 1/len(self.tag_vocab)
            p = (self.tag_trans[key] + lam * len(self.tag_vocab) * p_z)/ (self.tag_tot[key[0]]+ lam * len(self.tag_vocab))
            # except ZeroDivisionError:
            #     p = 0

            # Update probability dictinoaries
            self.trans_prob[(key[1], key[0])] = Prob(p)
        # Compute emission probabilities
        for key in self.tag_to_ob.keys():
            #try:
            #p_z = (self.obs_tot[key[1]] + lam)/ (self.word_amount_obs + lam * len(self.observation_vocab))
            p_z = (self.obs_tot[key[1]] + lam)/ (self.obs_tot[key[1]] + lam * len(self.observation_vocab))
            #p_z = 1/len(self.observation_vocab)
            p = (self.tag_to_ob[key] + lam * len(self.observation_vocab) * p_z)/ (self.obs_tot[key[0]] + lam * len(self.observation_vocab))
            # except ZeroDivisionError:
            #     p = 0
            # Update probability dictionary
            self.emiss_prob[(key[1], key[0])] = Prob(p)

    def posterior_decode(self, tags, observations):
        out_list = []
        alpha = {}
        alpha['###', 0] = Prob(1)
        tag_list_no_hashtag = [tag for tag in list(self.tag_vocab) if tag != '###']
        for i in range(1, len(observations)):
            obs = observations[i]
            for s in self.possible_tags[obs]:
                alpha[s, i] = Prob(0) #we need to do this in order to make a comparison because at the start there is nothing there
                for t in self.possible_tags[observations[i - 1]]:
                    # Compute potential value for current "cell"
                    p = self.trans_prob[s, t] * self.emiss_prob[obs, s]
                    alpha[s,i] += alpha[t, i - 1] * p

        Z = alpha["###", len(observations) - 1]

        beta = defaultdict(lambda: Prob(0))
        beta["###", len(observations) - 1] = Prob(1)

        for i in range(len(observations) - 1, 0, -1):
            most_prob_tag = 0
            most_prob = Prob(0)
            for s in self.possible_tags[observations[i]]:
                posterior = alpha[s, i] * beta[s, i] / Z
                for t in self.possible_tags[observations[i-1]]:
                    p = self.trans_prob[s, t] * self.emiss_prob[observations[i], s]
                    beta[t, i - 1] += beta[s, i] * p
                if posterior >= most_prob:
                    most_prob_tag = s
                    most_prob = posterior
            out_list.append(most_prob_tag)

        out_list.append("###")
        out_list.reverse()
        return out_list

    def viterbi(self, tags, observations):
        #table = defaultdict(lambda: Prob(0))
        table = {}
        backpointer = {}
        table['###', 0] = Prob(1)
        tag_list_no_hashtag = [tag for tag in list(self.tag_vocab) if tag != '###']
        for i in range(1, len(observations)):
            obs = observations[i]
            for s in self.possible_tags[obs]:
                table[s, i] = Prob(0) #we need to do this in order to make a comparison because at the start there is nothing there
                for t in self.possible_tags[observations[i - 1]]:
                    # Compute potential value for current "cell"
                    p = self.trans_prob[s, t] * self.emiss_prob[obs, s]
                    mu = table[t, i - 1] * p
                    if mu > table[s, i]:
                        table[s, i] = mu
                        backpointer[s, i] = t

        final_max = Prob(0)
        max_key = None
        for t in self.tag_vocab:
           val = table.get((t, len(observations) - 2), Prob(0))
           if val >= final_max:
               final_max = val
               max_key = (t, len(observations) - 2)

        # Add last backpointer entry
        backpointer['###', len(observations) - 1] = max_key[0]

        # for key in table:
        #     print(key, '--->', table[key].get_log_prob())
        # print()
        #
        # for key in backpointer:
        #     print(key, '--->', backpointer[key])
        # Trace back to make final predictions
        start = backpointer['###', len(observations) - 1]
        ind = start
        result = ['###', start]
        for i in range(len(observations) - 2, 1, -1):
            next = backpointer[ind, i]
            result.append(next)
            ind = next
        result.append('###')
        result = result[::-1]
        return result

    def intialize_dictionaries(self, tags, obs, lam):
        for observe in self.observation_vocab:
            for tag in self.tag_vocab:
                self.emiss_prob[observe, tag] = Prob(lam / (self.tag_tot[tag] + lam * len(self.observation_vocab)))
        for tag1 in self.tag_vocab:
            for tag2 in self.tag_vocab:
                self.trans_prob[tag2, tag1] = Prob(lam / (self.tag_tot[tag1] + lam * len(self.tag_vocab)))
        for tag in self.tag_vocab:
            self.initial_prob[tag] = Prob(lam / (self.tag_tot[tag] + lam * len(self.tag_vocab)))

    def train(self, train_file, lam, backoff):
        tags, obs = self.read_train_file(train_file)
        if backoff:
            self.counting(tags, obs)
            self.intialize_dictionaries(tags, obs, lam)
            self.backoff_add_lambda(lam)
            self.emiss_prob['###', '###'] = Prob(1)
        else:
            self.counting(tags, obs)
            self.intialize_dictionaries(tags, obs, lam)
            self.add_lambda(lam)
            self.emiss_prob['###', '###'] = Prob(1)

    def calc_perplexity_tagged(self, tags, obs):
        n = len(obs) - 1
        perplexity = Prob(1)
        for i in range(1, len(tags)):
            try:
                perplexity *= self.emiss_prob[obs[i], tags[i]] * self.trans_prob[tags[i], tags[i - 1]]
            except KeyError:
                return 0
        return np.exp(-perplexity.get_log_prob() / n)

    def calc_accs(self, result, tags, obs):
        # Accuracy
        right = 0
        total = 0
        number_hash = 0 #we need this so that when we get acc we do not divide by a larger number than we should be
        known = 0
        novel = 0
        known_total = 0
        novel_total = 0
        for i in range(len(obs)):
            if result[i] == "###":
                number_hash += 1
            else:
                if result[i] == tags[i]:
                    right += 1
                if obs[i] == "OOV":
                    novel_total += 1
                    if tags[i] == result[i]:
                        novel += 1
                else:
                    known_total +=1
                    if tags[i] == result[i]:
                        known += 1
                total += 1

        acc = right * 100 /(total)
        known_acc = 0
        novel_acc = 0
        if known_total != 0:
            known_acc = known * 100 / (known_total)
        if novel_total != 0:
            novel_acc = novel * 100 / (novel_total)

        return acc, known_acc, novel_acc

    def predict(self, file_name):
        tags, obs = self.read_test_file(file_name)
        result = self.viterbi(tags, obs)

        perp = self.calc_perplexity_tagged(tags, obs)
        acc, known_acc, novel_acc = self.calc_accs(result, tags, obs)

        print("Model perplexity per tagged test word: {:0.3f}".format(perp))
        print("Tagging accuracy (Viterbi decoding): {:0.2f}% (known: {:0.2f}% novel: {:0.2f}%)".format(acc, known_acc, novel_acc))

    def predict_pd(self, file_name):
        tags, obs = self.read_test_file(file_name)
        result = self.posterior_decode(tags, obs)
        perp = self.calc_perplexity_tagged(tags, obs)
        acc, known_acc, novel_acc = self.calc_accs(result, tags, obs)

        with open('test-output', 'w') as f:
            for o, r in zip(obs, result):
                f.write('{}/{}\n'.format(o, r))

        #print("Model perplexity per tagged test word: {:0.3f}".format(perp))
        print("Tagging accuracy (posterior decoding): {:0.2f}% (known: {:0.2f}% novel: {:0.2f}%)".format(acc, known_acc, novel_acc))


def main():
    # Read command line arguments
    train_file = sys.argv[1]
    test_file = sys.argv[2]
    # Create and train Tagger object
    tagger = Tagger()
    tagger.train(train_file, .001, False)
    # Predict
    #tagger.predict(test_file)
    tagger.predict(test_file)
def main_posterior():
    # Read command line arguments
    train_file = sys.argv[1]
    test_file = sys.argv[2]
    # Create and train Tagger object
    tagger = Tagger()
    tagger.train(train_file, 5, False)
    # Predict
    tagger.predict(test_file)
    tagger.predict_pd(test_file)
if __name__ == '__main__':
    #main()
    main_posterior()
