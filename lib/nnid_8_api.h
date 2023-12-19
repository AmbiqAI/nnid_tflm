#ifndef __nnid_8_API_H
#define __nnid_8_API_H

#include "ns_model.h"

#define nnid_8_STATUS_SUCCESS 0
#define nnid_8_STATUS_FAILURE -1
#define nnid_8_STATUS_INVALID_HANDLE 1
#define nnid_8_STATUS_INVALID_VERSION 2
#define nnid_8_STATUS_INVALID_CONFIG 3
#define nnid_8_STATUS_INIT_FAILED 4

extern int
nnid_8_init(ns_model_state_t *ms);

extern int
nnid_8_minimal_init(ns_model_state_t *ms);

#endif
