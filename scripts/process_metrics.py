#!/usr/bin/python3

import sys
import time
import os
import csv
import pandas
# from decimal import *
# getcontext().prec=9
#sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
#from toexcel import panda_to_excel

# Get # of processors
NUM_PROCS=2
try:
    process = subprocess.Popen( "lscpu | grep -E 'Core\(s\) per socket:'", shell=True, 
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
    )
    output, _ = process.communicate()

    NUM_PROCS= int(output.strip().split(":")[1].split(" ")[-1])
    # print('numproc: ', NUM_PROCS)
except Exception as e:
    NUM_PROCS=2

def long_metrics(pfile, runs): # for 02 benchmarks --> get the difference between time measurements

    csvdata = [0.0]

    # AVERAGE DIFF B/W
    PACC = 0.0
    line_cnt = 0
    prevline = ''

    PARA_AVGDIFFS = [0.0]*int(runs)
    cntr = 0

    with open(pfile, "r") as f:
        data = csv.reader(f)
        for row in data:
            if row[0][0] == '#' or row[0][0] == 't': continue
            if row[0][0] == '*':
                if PACC > 0.0: 
                    # accum. data in some way
                    PARA_AVGDIFFS[cntr] = PACC / float(line_cnt)
                    cntr+=1
                if prevline and line_cnt == 0:
                    PARA_AVGDIFFS[cntr] = float(prevline) / 1.0
                    cntr+=1
                # reset individual run metrics
                PACC = 0.0
                line_cnt = 0
                prevline = ''
                continue
            if prevline:
                # get difference b/w this and prev time
                PACC += ( float(row[0]) - float(prevline) )
                line_cnt += 1
            prevline = row[0]

    # if int(runs) != int(line_cnt): print('runs: ', runs, 'linecnt: ', line_cnt)
    # print(PARA_AVGDIFFS)
    df = pandas.DataFrame([PARA_AVGDIFFS])

    mean_value=float(df[0].mean())
    median_value= float(df[0].median())
    max_value=float(df[0].max())
    min_value=float(df[0].min())
    if min_value <= 0.0: min_value = -1.0
    # print('PANDAS: ', mean_value, median_value, max_value, min_value, float(median_value/min_value), float( max_value/min_value))

    # AVERAGE

    csvdata = [mean_value, median_value, max_value, min_value, float(float(median_value)/float(min_value)), float(float(max_value)/float(min_value))]

    AVGDIFFOVERALL = sum(PARA_AVGDIFFS)/len(PARA_AVGDIFFS)
    #AVGDIFFOVERALL = AVGDIFFOVERALL*1000000000.0   

    csvdata[0] = AVGDIFFOVERALL

    return csvdata

def short_metrics(pfile, runs): #

    # METRICS

    # average , median, max, min, med/min, max/min
    df=pandas.read_csv(pfile, header=None)
    # print(df[0])

    mean_value=float(df[0].mean())
    median_value= float(df[0].median())
    max_value=float(df[0].max())
    min_value=float(df[0].min())

    if min_value <= 0.0: min_value = -1.0
        # min_value=float(max_value)
        # with open(pfile, "r") as f:
        #     data = csv.reader(f)
        #     for row in data:
        #         print(float(row[0]))
        #         if row[0][0] == '*' or row[0][0] == '#': continue
        #         else:
        #             if float(row[0]) < min_value:
        #                 min_value = float(row[0])

    # print('PANDAS: ', mean_value, median_value, max_value, min_value)
    # print()
    # AVERAGE

    csvdata = [mean_value, median_value, max_value, min_value, float(float(median_value)/float(min_value)), float(float(max_value)/float(min_value))]
   
    return csvdata

def main():

    csvdata = []
    # print('python: ', sys.argv)

    # sys.argv = [ ./X , filename, #runs ]
    if '2' in sys.argv[1][0:7]:
        csvdata = long_metrics(sys.argv[1], sys.argv[4])
    else:
        csvdata = short_metrics(sys.argv[1], sys.argv[4])


    bk = sys.argv[1].split('/')[1][0:3]
    second_half=[bk, int(sys.argv[2]), sys.argv[3], int(sys.argv[4]), int(sys.argv[5]), sys.argv[6],sys.argv[7],sys.argv[8],sys.argv[9],sys.argv[10]]

    # average , median, max, min, med/min, max/min, bk , nt , lang, runs , iters , clock , fcn , [bind settings] , [other settings] , arch  
    #   ...       ...   ...  ...   ...      ...     a1   a2    a3     a4      a5      a6     a7                 a8              a9     a10


    #try:
    # panda_to_excel(bk, sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[1], sys.argv[6], csvdata[0],sys.argv[7])

    #except Exception as e:
    #print('error in printing to sheet')

    csvdata = csvdata + second_half

    for idx,i in enumerate(csvdata):
        print(i,end='')
        if csvdata[idx] == csvdata[-1]:
            continue
        print(',',end='')

    print()

    return

# average outputs
# computing any other metrics
#   time
#   “overhead”
#   code size?

if __name__== '__main__':
    main()
