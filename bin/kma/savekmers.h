/* Philip T.L.C. Clausen Jan 2017 plan@dtu.dk */

/*
 * Copyright (c) 2017, Philip Clausen, Technical University of Denmark
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *		http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
#define _XOPEN_SOURCE 600
#include <stdio.h>
#include "compdna.h"
#include "hashmapkma.h"
#include "penalties.h"
#include "qseqs.h"

#ifndef SAVEKMERS
typedef struct kmerScan_thread KmerScan_thread;
struct kmerScan_thread {
	pthread_t id;
	int num;
	int exhaustive;
	int spltDB;
	int bestScore;
	int bestScore_r;
	int *bestTemplates;
	int *bestTemplates_r;
	FILE *inputfile;
	HashMapKMA *templates;
	CompDNA *qseq;
	CompDNA *qseq_r;
	Qseqs *header;
	Penalties *rewards;
	struct kmerScan_thread *next;
};
#define SAVEKMERS 1;
#endif

/* pointers to combine functions */
void (*ankerPtr)(int*, int*, int*, int*, unsigned**, unsigned**, int*, CompDNA*, int, int, int, int, const Qseqs*, volatile int*);
void (*kmerScan)(const HashMapKMA *, const Penalties *, int*, int*, int*, int*, CompDNA*, CompDNA*, const Qseqs*, int*, const int, volatile int*);
void (*save_kmers_pair)(const HashMapKMA *, const Penalties *, int*, int*, int*, int*, int*, int*, CompDNA*, CompDNA*, const Qseqs*, const Qseqs*, int*, const int, volatile int*);
int (*get_kmers_for_pair_ptr)(const HashMapKMA *, const Penalties *, int *, int *, int *, int *, CompDNA *, int *, int);

int loadFsa(CompDNA *qseq, Qseqs *header, FILE *inputfile);
void * save_kmers_threaded(void *arg);
int get_kmers_for_pair(const HashMapKMA *templates, const Penalties *rewards, int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, CompDNA *qseq, int *extendScore, const int exhaustive);
int get_kmers_for_pair_count(const HashMapKMA *templates, const Penalties *rewards, int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, CompDNA *qseq, int *extendScore, const int exhaustive);
int get_kmers_for_pair_Sparse(const HashMapKMA *templates, const Penalties *rewards, int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, CompDNA *qseq, int *extendScore, const int exhaustive);
int get_kmers_for_pair_pseoudoSparse(const HashMapKMA *templates, const Penalties *rewards, int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, CompDNA *qseq, int *extendScore, const int exhaustive);
void getFirstForce(int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, int *regionTemplates, int *regionScores);
int getSecondForce(int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, int *regionTemplates, int *regionScores);
int getFirstPen(int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, int *regionTemplates, int *regionScores);
int getSecondPen(int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, int *regionTemplates, int *regionScores, int bestScore, int PE);
int getF_Best(int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, int *regionTemplates);
int getR_Best(int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, int *regionTemplates);
void save_kmers_Sparse(const HashMapKMA *templates, const Penalties *rewards, int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, CompDNA *qseq, CompDNA *qseq_r, const Qseqs *header, int *extendScore, const int exhaustive, volatile int *excludeOut);
void save_kmers_pseuodeSparse(const HashMapKMA *templates, const Penalties *rewards, int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, CompDNA *qseq, CompDNA *qseq_r, const Qseqs *header, int *extendScore, const int exhaustive, volatile int *excludeOut);
void save_kmers(const HashMapKMA *templates, const Penalties *rewards, int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, CompDNA *qseq, CompDNA *qseq_r, const Qseqs *header, int *extendScore, const int exhaustive, volatile int *excludeOut);
int save_kmers_intCount(const HashMapKMA *templates, int *bestTemplates, int *Score, CompDNA *qseq, unsigned *values, unsigned pos, const unsigned shifter);
void save_kmers_count(const HashMapKMA *templates, const Penalties *rewards, int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, CompDNA *qseq, CompDNA *qseq_r, const Qseqs *header, int *extendScore, const int exhaustive, volatile int *excludeOut);
void save_kmers_unionPair(const HashMapKMA *templates, const Penalties *rewards, int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, int *regionTemplates, int *regionScores, CompDNA *qseq, CompDNA *qseq_r, const Qseqs *header, const Qseqs *header_r, int *extendScore, const int exhaustive, volatile int *excludeOut);
void save_kmers_penaltyPair(const HashMapKMA *templates, const Penalties *rewards, int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, int *regionTemplates, int *regionScores, CompDNA *qseq, CompDNA *qseq_r, const Qseqs *header, const Qseqs *header_r, int *extendScore, const int exhaustive, volatile int *excludeOut);
void save_kmers_forcePair(const HashMapKMA *templates, const Penalties *rewards, int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, int *regionTemplates, int *regionScores, CompDNA *qseq, CompDNA *qseq_r, const Qseqs *header, const Qseqs *header_r, int *extendScore, const int exhaustive, volatile int *excludeOut);
void save_kmers_HMM(const HashMapKMA *templates, const Penalties *rewards, int *bestTemplates, int *bestTemplates_r, int *Score, int *Score_r, CompDNA *qseq, CompDNA *qseq_r, const Qseqs *header, int *extendScore, const int exhaustive, volatile int *excludeOut);
void ankerAndClean(int *regionTemplates, int *Score, int *Score_r, int *template_lengths, unsigned **VF_scores, unsigned **VR_scores, int *tmpNs, CompDNA *qseq, int HIT, int bestScore, int start_cut, int end_cut, const Qseqs *header, volatile int *excludeOut);
void ankerAndClean_MEM(int *regionTemplates, int *Score, int *Score_r, int *template_lengths, unsigned **VF_scores, unsigned **VR_scores, int *tmpNs, CompDNA *qseq, int HIT, int bestScore, int start_cut, int end_cut, const Qseqs *header, volatile int *excludeOut);
