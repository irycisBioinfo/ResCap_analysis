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
#include <stdio.h>
#include "compdna.h"
#include "qseqs.h"

void (*printPtr)(int*, CompDNA*, int, const Qseqs*);
void (*printPairPtr)(int*, CompDNA*, int, const Qseqs*, CompDNA*, int, const Qseqs*);
void (*deConPrintPtr)(int*, CompDNA*, int, const Qseqs*);
void print_ankers(int *out_Tem, CompDNA *qseq, int rc_flag, const Qseqs *header);
void print_ankers_Sparse(int *out_Tem, CompDNA *qseq, int rc_flag, const Qseqs *header);
int find_contamination(int *out_Tem, const int contamination);
int find_contamination2(int *out_Tem, const int contamination);
void deConPrint(int *out_Tem, CompDNA *qseq, int rc_flag, const Qseqs *header);
void deConPrintPair(int *out_Tem, CompDNA *qseq, int bestScore, const Qseqs *header, CompDNA *qseq_r, int bestScore_r, const Qseqs *header_r);
void printPair(int *out_Tem, CompDNA *qseq, int bestScore, const Qseqs *header, CompDNA *qseq_r, int bestScore_r, const Qseqs *header_r);
int get_ankers(int *out_Tem, CompDNA *qseq, Qseqs *header, FILE *inputfile);
