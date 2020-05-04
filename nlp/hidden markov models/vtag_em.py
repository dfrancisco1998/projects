import math
import re
import sys
import numpy as np
from pathlib import Path
from typing import Any, Counter, Dict, List, Optional, Set, Tuple, Union
from scipy.special import logsumexp
from collections import Counter, defaultdict
from Prob import Prob
import copy
import time

class Tagger:

    def __init__(self):
        #these will be dictionaries - key by tuple s.t. if P(H|start) --> (H, start) is the key and we store the prob as a values
        #THESE ARE THE CUR
        self.emiss_prob = dict()
        self.trans_prob = dict()
        self.lambda_ = 0
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

        self.new_tags_to_obs = {}
        self.new_tag_trans = {}
        self.new_tag_tot = {}


        self.raw_vocab = set() #this holds novel words that appeared in raw but no in train
        #THESE ARE THE NEW
        self.xi = {}
        self.gamma = {}
        self.raw = None

        self.last_to_obs = {}
        self.first_to_obs = {}

        self.Z = Prob(0)

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

    def read_raw(self, file_name):
        raw = []
        with open(file_name, "r") as f:
            for line in f:
                line = line.strip()
                raw.append(line)
                if line not in self.observation_vocab:
                    self.possible_tags[line] = set([tag for tag in list(self.tag_vocab) if tag != '###'])
                    self.observation_vocab.add(line)
                    self.raw_vocab.add(line)
        return raw

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
    def add_lambda(self):

        # Compute transition probabilities
        for key in self.tag_trans.keys():
            #try:
            p = (self.tag_trans[key] + self.lambda_) / (self.tag_tot[key[0]] + (self.lambda_ * len(self.tag_vocab)))
            # Update probability dictionaries
            self.trans_prob[(key[1], key[0])] = Prob(p)


        # Compute emission probabilities
        for key in self.tag_to_ob.keys():
            #try:
            p = (self.tag_to_ob[key] + self.lambda_) / (self.tag_tot[key[0]] + (self.lambda_ * len(self.observation_vocab)))
            # Update probability dictionary
            self.emiss_prob[(key[1], key[0])] = Prob(p)

    def viterbi(self, tags, observations):
        #table = defaultdict(lambda: Prob(0))
        table = {}
        backpointer = {}
        table['###', 0] = Prob(1)
        tag_list_no_hashtag = [tag for tag in list(self.tag_vocab) if tag != '###']
        for i in range(1, len(observations)):
            obs = observations[i]
            for s in self.possible_tags[obs]:
                #print(self.possible_tags[obs])
                table[s, i] = Prob(0) #we need to do this in order to make a comparison because at the start there is nothing there
                for t in self.possible_tags[observations[i - 1]]:
                    #Compute potential value for current "cell"

                    #These print statemtns here were to see whcih one was causing a zero
                    # print("emiss: ", self.emiss_prob[obs, s], obs, s)
                    # print("trans: ", self.trans_prob[s, t], s, t)
                    p = self.trans_prob[s, t] * self.emiss_prob[obs, s]
                    mu = table[t, i - 1] * p
                    if mu >= table[s, i]:
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

    def initialize_dictionaries(self, tags, obs):
        for observe in self.observation_vocab:
            for tag in self.tag_vocab:
                self.emiss_prob[observe, tag] = Prob(self.lambda_ / (self.tag_tot[tag] + self.lambda_ * len(self.observation_vocab)))
        for tag1 in self.tag_vocab:
            for tag2 in self.tag_vocab:
                self.trans_prob[tag2, tag1] = Prob(self.lambda_ / (self.tag_tot[tag1] + self.lambda_ * len(self.tag_vocab)))

    def train(self, train_file, lam, backoff, raw_file):
        self.lambda_ = lam
        tags, obs = self.read_train_file(train_file)

        if backoff:
            #self.backoff_add_lambda(self.lambda_)
            pass
        else:
            self.counting(tags, obs)

            self.raw = self.read_raw(raw_file)
            self.initialize_dictionaries(tags, obs)
            self.add_lambda()


            self.emiss_prob['###', '###'] = Prob(1)

    def calc_perplexity_tagged(self, tags, obs):
        n = len(obs) - 1
        perplexity = Prob(1)
        for i in range(1, len(tags)):
            try:
                if self.emiss_prob[obs[i], tags[i]] == Prob(0):
                    self.emiss_prob[obs[i], tags[i]] = Prob(1/len(self.observation_vocab))
                if self.trans_prob[tags[i], tags[i - 1]] == Prob(0):
                    self.trans_prob[tags[i], tags[i - 1]] = Prob(1/len(self.tag_vocab))
                # print(self.emiss_prob[obs[i], tags[i]])
                # print(self.trans_prob[tags[i], tags[i - 1]])
                perplexity *= self.emiss_prob.get((obs[i], tags[i]), Prob(1/len(self.observation_vocab))) * self.trans_prob.get((tags[i], tags[i - 1]), Prob(1/len(self.tag_vocab)))

            except KeyError:
            #     if (obs[i], tags[i]) not in self.emiss_prob:
            #         perplexity *= Prob(1/len(self.observation_vocab))
            #     if (tags[i], tags[i - 1]) not in self.trans_prob:
            #         perplexity *= Prob(1/self.tag_vocab)
                pass
                # print('[ERROR] KeyError on perplexity: ')
                # if (obs[i], tags[i]) not in self.emiss_prob:
                #     print('Emission Error: ', (obs[i], tags[i]))
                #
                # if (tags[i], tags[i - 1]) not in self.trans_prob:
                #     print('Transition Error: ', (tags[i], tags[i - 1]))

                #quit()
                #return 0
        return np.exp(-perplexity.get_log_prob() / n)

    def calc_perplexity_untagged(self):
        perp = Prob(1)
        perp = np.exp(-1* self.Z.get_log_prob()/ len(self.raw) - 1)
        return perp

    def calc_accs(self, result, tags, obs):
        # Accuracy
        right = 0
        total = 0
        number_hash = 0 #we need this so that when we get acc we do not divide by a larger number than we should be
        known = 0
        novel = 0
        known_total = 0
        novel_total = 0
        seen_total = 0
        seen = 0
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

                if obs[i] in self.raw_vocab:
                    seen_total += 1
                    if tags[i] == result[i]:
                        seen += 1

                total += 1

        acc = right * 100 /(total)
        known_acc = 0
        novel_acc = 0
        seen_acc = 0
        if known_total != 0:
            known_acc = known * 100 / (known_total)
        if novel_total != 0:
            novel_acc = novel * 100 / (novel_total)
        if seen_total != 0:
            seen_acc = seen * 100 / (seen_total)
        return acc, known_acc, novel_acc, seen_acc

    def predict(self, file_name):
        tags, obs = self.read_test_file(file_name)
        result = self.viterbi(tags, obs)

        perp = self.calc_perplexity_tagged(tags, obs)
        acc, known_acc, novel_acc, seen_acc = self.calc_accs(result, tags, obs)

        print("Model perplexity per tagged test word: {:0.3f}".format(perp))
        print("Tagging accuracy (Viterbi decoding): {:0.2f}% (known: {:0.2f}% novel: {:0.2f}%)".format(acc, known_acc, novel_acc))

    def predict_em(self, tags, obs):
        #print('running viterbi')
        result = self.viterbi(tags, obs)
        #print('calculating perplexity/accuracy')
        perp= self.calc_perplexity_tagged(tags, obs)
        #perp_untagged = self.calc_perplexity_untagged()
        acc, known_acc, novel_acc, seen_acc = self.calc_accs(result, tags, obs)

        print("Model perplexity per tagged test word: {:0.3f}".format(perp))
        print("Tagging accuracy (Viterbi decoding): {:0.2f}% (known: {:0.2f}% seen: {:0.2f}% novel: {:0.2f}%)".format(acc, known_acc,seen_acc,  novel_acc))

    def e_step(self):
        observations = self.raw
        self.last_to_obs = {}
        self.first_to_obs = {}
        self.xi = {}
        self.gamma = {}
        alpha = {}
        alpha['###', 0] = Prob(1)
        for i in range(1, len(observations)):
            obs = observations[i]
            for s in self.possible_tags[obs]:
                alpha[s, i] = Prob(0) #we need to do this in order to make a comparison because at the start there is nothing there
                for t in self.possible_tags[observations[i - 1]]:
                    # Compute potential value for current "cell"
                    p = self.trans_prob[s, t] * self.emiss_prob[obs, s]
                    alpha[s,i] += alpha[t, i - 1] * p

        self.Z = alpha["###", len(observations) - 1]
        beta = defaultdict(lambda: Prob(0))
        beta["###", len(observations) - 1] = Prob(1)
        n = len(observations) - 1
        for i in range(len(observations) - 1, 0, -1):
            obs = observations[i]
            for s in self.possible_tags[observations[i]]:
                tag = s
                #the gamma is our emmison
                self.gamma[i, observations[i], s] = alpha[s, i] * beta[s, i] / self.Z
                #just to not get a key error when we add
                if tag not in self.new_tag_tot:
                    self.new_tag_tot[tag] = Prob(1 / len(self.tag_vocab))

                #to not get a key error when we add
                if (obs, tag) not in self.new_tags_to_obs:
                    self.new_tags_to_obs[obs, tag] = Prob(1 / len(self.observation_vocab))


                #this is marginalizing
                self.new_tags_to_obs[obs, tag] += self.gamma.get((i, obs, tag), Prob(0))
                self.new_tag_tot[tag] += self.gamma.get((i, obs, tag), Prob(0))

                #stuff for initial and end probs
                if i == n - 1:
                    if tag not in self.last_to_obs:
                        self.last_to_obs[tag] = Prob(1 / len(self.tag_vocab))
                    self.last_to_obs[tag] = max(self.gamma.get((i, obs, tag), Prob(0)), self.last_to_obs[tag])

                if i == 1:
                    if tag not in self.first_to_obs:
                        self.first_to_obs[tag] = Prob(1 / len(self.tag_vocab))
                    self.first_to_obs[tag] = max(self.gamma.get((i, obs, tag), Prob(0)), self.first_to_obs[tag])

                #this is for transition
                for t in self.possible_tags[observations[i-1]]:
                    #make sure we do not have error
                    if tag not in self.new_tag_tot:
                        self.new_tag_tot[tag] = Prob(1 / len(self.tag_vocab))
                    if (t, s) not in self.new_tag_trans:
                        self.new_tag_trans[t, s] = Prob(1 / len(self.tag_vocab))
                    #calculating beta and xi as well as constanly addint in probs
                    p = self.trans_prob[s, t] * self.emiss_prob[observations[i], s]
                    beta[t, i - 1] += beta[s, i] * p
                    self.xi[i, t, s] = (alpha[t, i-1] * p * beta[s, i]) / self.Z
                    self.new_tag_trans[t, s] += self.xi.get((i, t, s), Prob(0))

    def m_step(self):
        for e in self.emiss_prob:
            #the if statment is to prevent overwrtting with zeros or nan
            if e not in self.new_tags_to_obs or e[1] not in self.new_tag_tot:
                self.emiss_prob[e] = Prob(0)

            elif self.new_tags_to_obs[e] != Prob(0) and self.new_tag_tot[e[1]] != Prob(0):
                self.emiss_prob[e] = self.new_tags_to_obs[e] / self.new_tag_tot[e[1]]
            else:
                self.emiss_prob[e] = Prob(self.lambda_ / (self.tag_tot[e[1]] + self.lambda_ * len(self.observation_vocab)))

        self.emiss_prob["###", "###"] = Prob(1)
        # Update transition probabilities
        for t in self.trans_prob:
            if t[0] == "###" and t[1] == "###":
                self.trans_prob[t] = Prob(0)
            elif t[0] == "###" and t[1] != "###":
                #l = self.last_to_obs[t[1]]
                #k = self.new_tag_tot[t[1]]
                self.trans_prob[t] = self.last_to_obs.get(t[1], Prob(1 / len(self.tag_vocab))) / self.new_tag_tot.get(t[1], Prob(1))
            elif  t[0] != "###" and t[1] == "###":
                self.trans_prob[t] = self.first_to_obs.get(t[0], Prob(1 / len(self.tag_vocab)))

            else:
                #the if statment is to prevent overwrtting with zeros or nan
                if self.new_tag_trans[t[1],t[0]] != Prob(0) and self.new_tag_tot[t[1]] != Prob(0):
                    self.trans_prob[t] = self.new_tag_trans[t[1],t[0]] / self.new_tag_tot[t[1]]
                else:
                    self.trans_prob[t] = Prob(self.lambda_ / (self.tag_tot[t[1]] + self.lambda_ * len(self.tag_vocab)))

        return

    def em_algo(self, test_file, raw_file):
        raw = self.raw
        tags, observations = self.read_test_file(test_file)
        self.predict_em(tags, observations)
        for i in range(10):
            t1 = time.time()
            #print('starting e step')
            self.e_step()
            #print("after e step", time.time() - t1)
            t1 = time.time()
            #print('starting m step')
            self.m_step()
            #print("finish m step", time.time() - t1)
            t1 = time.time()
            #print('making prediction')
            untagged_perp = self.calc_perplexity_untagged()
            print("Iteration {}: Model perplexity per untagged raw word: {:0.3f}".format(i, untagged_perp))
            self.predict_em(tags, observations)
        return


def main_em():
    train_file = sys.argv[1]
    test_file = sys.argv[2]
    raw_file = sys.argv[3]
    # Create and train Tagger object
    tagger = Tagger()
    tagger.train(train_file, .001, False, raw_file)
    tagger.em_algo(test_file, raw_file)



def main():
    # Read command line arguments
    train_file = sys.argv[1]
    test_file = sys.argv[2]
    # Create and train Tagger object
    tagger = Tagger()
    tagger.train(train_file, 1, False)
    # Predict
    #tagger.predict(test_file)
    tagger.predict_pd(test_file)


if __name__ == '__main__':
    main_em()
