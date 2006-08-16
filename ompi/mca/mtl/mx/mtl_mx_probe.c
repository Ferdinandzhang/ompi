/*
 * Copyright (c) 2004-2006 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2006 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ompi_config.h"

#include "ompi/request/request.h"
#include "ompi/communicator/communicator.h"

#include "mtl_mx.h"
#include "mtl_mx_types.h"
#include "mtl_mx_request.h"


int
ompi_mtl_mx_iprobe(struct mca_mtl_base_module_t* mtl, 
                   struct ompi_communicator_t *comm,
                   int src,
                   int tag,
                   int *flag,
                   struct ompi_status_public_t *status)
{
    uint32_t result;
    mx_return_t ret;
    mx_status_t mx_status;
    uint64_t match_bits;
    uint64_t mask_bits;

    MX_SET_RECV_BITS(match_bits, 
                     mask_bits,
                     comm->c_contextid,
                     src,
                     tag);

    ret = mx_iprobe(ompi_mtl_mx.mx_endpoint,
                    match_bits,
                    mask_bits,
                    &mx_status,
                    &result);
    if (MX_SUCCESS != ret) {
        opal_output(0, "Error in mx_iprobe (error %s)\n", mx_strerror(ret));
        return OMPI_ERROR;
    }

    if (result) {
        status->MPI_ERROR = OMPI_SUCCESS;
        MX_GET_SRC(mx_status.match_info, status->MPI_SOURCE);
        MX_GET_TAG(mx_status.match_info, status->MPI_TAG); 
        status->_count = mx_status.msg_length;

        *flag = 1;
    } else {
        *flag = 0;
    }

    return OMPI_SUCCESS;
}
