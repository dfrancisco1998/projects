import argparse
import sys
import random
import numpy as np
import math
from scipy.special import logsumexp

def read_file(f):
    grammar = []
    with open(f) as fl:
        for line in fl:
            tokens = line.split()
            grammar.append(tokens)
    return grammar

def make_dict(g):
    #non terminals to nonterminals
    basic_grammar = dict()
    #terminals to nonterminals
    term_to_non = dict()
    prob = dict()
    for l in g:
        if l[1] in basic_grammar.keys():
            basic_grammar[l[1]].append((tuple(l[2:len(l)]), 0-math.log2(float(l[0]))))
        else:
            basic_grammar[l[1]] = []
            basic_grammar[l[1]].append((tuple(l[2:len(l)]), 0-math.log2(float(l[0]))))
    for key in basic_grammar.keys():
        for rule_prob in basic_grammar[key]:
            # for i in range(0, len(li), 2):
            if rule_prob[0] not in term_to_non.keys():
                term_to_non[rule_prob[0]] = []
            term_to_non[rule_prob[0]].append(key)

    return basic_grammar, prob, term_to_non

#so we need to go through all the combos and check if there is a rule that can connect the two
def compute_pairings(B, C, basic_grammar,prob, term_to_non, i, j, k):
    out = []
    for rule1 in B:
        b = rule1[0] # consitutent
        for rule2 in C:
            c = rule2[0]  # consitutent
            # print('b', b)
            # print('c', c)
            #quit()
            if (b, c) in term_to_non.keys():
                # print('term_to norm:', term_to_non[(b, c)])
                # out.append(term_to_non[(rule1[0], rule2[0])])
                # back_b = (i, k, rule1)
                # back_c = (k, j, rule2)
                back = (i, j, k, b, c)
                left_rules = term_to_non[(b, c)]
                # print('left rules:', left_rules)

                for r in left_rules:
                    out.append(tuple((r, back)))


    #print(out)
    return out

def cky(basic_grammar,prob, term_to_non, sentence):
    table = []
    for i in range(len(sentence) + 1):
        table.append([])
        for j in range(len(sentence)+ 1):
            table[i].append([])

    for i in range(1, len(sentence) + 1):
        if (sentence[i-1], ) in term_to_non.keys():
            iter = term_to_non[(sentence[i-1], )]
            for it in iter:
                # print('iter:', iter)\
                #print(it)
                table[i - 1][i].append((it, [(i-1, -1, -1, '', '')])) #list of tuples that has tuple[0] as terinal and tuple[1] as probs
                #print(table[i - 1][i])
        # quit()
    #print(table)
    table = np.asarray(table)
    table = table[0:-1, :]
    for j in range(2, len(sentence) + 1):
        for i in range(j-2, -1, -1):
            for k in range(i+1, j, 1):
                B_list = table[i][k]
                C_list = table[k][j] #rule representing table[k,j]
                # print('B:', i, k, B_list)
                # print('C:', k, j, C_list)
                if len(C_list) == 0 or len(B_list) == 0:
                    continue
                A = compute_pairings(B_list, C_list, basic_grammar, prob, term_to_non, i, j, k)
                # print('A:', A)
                # table[i][j].append(A)
                for new_rule in A:
                    # print('new rule:', new_rule)
                    new_constit = new_rule[0]
                    back = new_rule[1]
                    present = False
                    for x in range(len(table[i, j])):
                        rule = table[i, j][x]
                        constit = rule[0]
                        if constit == new_constit:
                            present = True
                            # print('rule[1]:', rule[1])
                            rule[1].append(back)
                            break
                    if not present:
                        backs = list([back])
                        # print('backs:', backs)
                        table[i, j].append((new_constit, backs))


    # for i in range(len(sentence)):
    #     for j in range(len(sentence)+1):
    #         print('(', i, j, '):', table[i, j])

    return table

def backwards_cky(cky_table, basic_grammar, sentence):
    parses = []
    probs = []
    i = 0
    j = len(sentence)
    trace_back(cky_table, basic_grammar, sentence, i, j, 'ROOT', parses, [''], [''], probs, 0)
    x = 0
    # for x in range(len(parses)):
    while x < len(parses):
        parse = parses[x]
        curr_prob = probs[x]
        x += 1
        for y in range(len(parse)):
            if isinstance(parse[y], tuple):
                tup = parse[y]
                i = tup[0]
                j = tup[1]
                lhs = tup[2]
                trace_back(cky_table, basic_grammar, sentence, i, j, lhs, parses, parse[0:y], parse[y+1:], probs, curr_prob)

                del parses[x-1]
                del probs[x - 1]
                x -= 1
                break

    parse_sents = []
    for parse in parses:
        parse_sent = ''
        for str in parse:
           parse_sent+=str
        if parse_sent not in parse_sents:
            parse_sents.append(parse_sent)

    # print('sents len:', len(parse_sents))
    # for parse_sent in parse_sents:
    #     print(parse_sent)
    # print(probs)
    # for prob in probs:
    #     print(2**(-prob))

    return parse_sents, probs


def trace_back(cky_table, basic_grammar, sentence, i, j, lhs, parses, l_parse, r_parse, probs, curr_prob):
    if j == -1:
        parses.append(l_parse + [sentence[i]] + r_parse)
        probs.append(curr_prob)

        # if i == -1:
        #     print('i==-1:', l_parse, r_parse)
    else:
        for sub in cky_table[i, j]:
            constit = sub[0]
            if constit == lhs:
                backs = sub[1]
                for back in backs:
                    i = back[0]
                    j = back[1]
                    k = back[2]
                    constit1 = back[3]
                    constit2 = back[4]
                    if k != -1:
                        parses.append(l_parse + [' (', constit, (i, k, constit1), (k, j, constit2), ')'] + r_parse)
                        for rhs in basic_grammar[constit]:
                            rhs_words = rhs[0]
                            if rhs_words[0] == constit1 and rhs_words[1] == constit2:
                                # print('probs.append:', curr_prob * float(rhs[1]))
                                probs.append(curr_prob + float(rhs[1]))
                                # print(constit, constit1, constit2, curr_prob, float(rhs[1]), 2**(-float(rhs[1])))
                    else:
                        parses.append(l_parse + [' (', constit, ' ', (i, k, constit1), ')'] + r_parse)
                        for rhs in basic_grammar[lhs]:
                            rhs_words = rhs[0]
                            if rhs_words[0] == sentence[i]:
                                probs.append(curr_prob + float(rhs[1]))
                                # print('probs.append:2', curr_prob * float(rhs[1]))



def best_parse(parses, probs):
    if len(parses) > 0:
        min_index = probs.index(min(probs))
        print('{:.3f}'.format(round(probs[min_index], 3)), parses[min_index][1:], sep='\t')
    else:
        print("-\tNOPARSE")



def total_weight(parses, probs):
    if (len(parses) > 0):
        sum = 0
        for i in range(len(probs)):
            sum += (2**(-probs[i]))
            # if i > 0:
            #     sum = np.logaddexp2(sum, probs[i])
        total = -math.log2(sum)
        print('{:.3f}'.format(round(total, 3)))

    else:
        print('-')



def main():
    MODE = sys.argv[1]
    grammar_file = sys.argv[2]
    sentence_file = sys.argv[3]
    gram = read_file(grammar_file)
    sentences = read_file(sentence_file)
    basic_grammar, prob, term_to_non = make_dict(gram)
    # print(basic_grammar)
    # print()
    # print(term_to_non)

    for sent in sentences:
        # print(sent)
        if len(sent) > 0:
            cky_table = cky(basic_grammar, prob, term_to_non, sent)
            parses, probs = backwards_cky(cky_table, basic_grammar, sent)
            # print(probs)
            # for i in range(len(probs)):
            #     probs[i] = - math.log2(round(2**(-probs[i]), 10))
            if MODE == 'RECOGNIZER':
                print(len(parses) > 0)
            elif MODE == 'BEST-PARSE':
                best_parse(parses, probs)
            elif MODE == 'TOTAL-WEIGHT':
                total_weight(parses, probs)
            else:
                print('INVALID MODE')


if __name__ == '__main__':
    main()
