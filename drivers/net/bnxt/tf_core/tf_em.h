/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2019-2020 Broadcom
 * All rights reserved.
 */

#ifndef _TF_EM_H_
#define _TF_EM_H_

#include "tf_core.h"
#include "tf_session.h"

#define TF_HACK_TBL_SCOPE_BASE 68
#define SUPPORT_CFA_HW_P4 1
#define SUPPORT_CFA_HW_P58 0
#define SUPPORT_CFA_HW_P59 0
#define SUPPORT_CFA_HW_ALL 0

#include "hcapi/hcapi_cfa_defs.h"

#define TF_HW_EM_KEY_MAX_SIZE 52
#define TF_EM_KEY_RECORD_SIZE 64

#define TF_EM_MAX_MASK 0x7FFF
#define TF_EM_MAX_ENTRY (128 * 1024 * 1024)

/**
 * Hardware Page sizes supported for EEM:
 *   4K, 8K, 64K, 256K, 1M, 2M, 4M, 1G.
 *
 * Round-down other page sizes to the lower hardware page
 * size supported.
 */
#define TF_EM_PAGE_SIZE_4K 12
#define TF_EM_PAGE_SIZE_8K 13
#define TF_EM_PAGE_SIZE_64K 16
#define TF_EM_PAGE_SIZE_256K 18
#define TF_EM_PAGE_SIZE_1M 20
#define TF_EM_PAGE_SIZE_2M 21
#define TF_EM_PAGE_SIZE_4M 22
#define TF_EM_PAGE_SIZE_1G 30

/* Set page size */
#define BNXT_TF_PAGE_SIZE TF_EM_PAGE_SIZE_2M

#if (BNXT_TF_PAGE_SIZE == TF_EM_PAGE_SIZE_4K)	/** 4K */
#define TF_EM_PAGE_SHIFT TF_EM_PAGE_SIZE_4K
#define TF_EM_PAGE_SIZE_ENUM HWRM_TF_CTXT_MEM_RGTR_INPUT_PAGE_SIZE_4K
#elif (BNXT_TF_PAGE_SIZE == TF_EM_PAGE_SIZE_8K)	/** 8K */
#define TF_EM_PAGE_SHIFT TF_EM_PAGE_SIZE_8K
#define TF_EM_PAGE_SIZE_ENUM HWRM_TF_CTXT_MEM_RGTR_INPUT_PAGE_SIZE_8K
#elif (BNXT_TF_PAGE_SIZE == TF_EM_PAGE_SIZE_64K)	/** 64K */
#define TF_EM_PAGE_SHIFT TF_EM_PAGE_SIZE_64K
#define TF_EM_PAGE_SIZE_ENUM HWRM_TF_CTXT_MEM_RGTR_INPUT_PAGE_SIZE_64K
#elif (BNXT_TF_PAGE_SIZE == TF_EM_PAGE_SIZE_256K)	/** 256K */
#define TF_EM_PAGE_SHIFT TF_EM_PAGE_SIZE_256K
#define TF_EM_PAGE_SIZE_ENUM HWRM_TF_CTXT_MEM_RGTR_INPUT_PAGE_SIZE_256K
#elif (BNXT_TF_PAGE_SIZE == TF_EM_PAGE_SIZE_1M)	/** 1M */
#define TF_EM_PAGE_SHIFT TF_EM_PAGE_SIZE_1M
#define TF_EM_PAGE_SIZE_ENUM HWRM_TF_CTXT_MEM_RGTR_INPUT_PAGE_SIZE_1M
#elif (BNXT_TF_PAGE_SIZE == TF_EM_PAGE_SIZE_2M)	/** 2M */
#define TF_EM_PAGE_SHIFT TF_EM_PAGE_SIZE_2M
#define TF_EM_PAGE_SIZE_ENUM HWRM_TF_CTXT_MEM_RGTR_INPUT_PAGE_SIZE_2M
#elif (BNXT_TF_PAGE_SIZE == TF_EM_PAGE_SIZE_4M)	/** 4M */
#define TF_EM_PAGE_SHIFT TF_EM_PAGE_SIZE_4M
#define TF_EM_PAGE_SIZE_ENUM HWRM_TF_CTXT_MEM_RGTR_INPUT_PAGE_SIZE_4M
#elif (BNXT_TF_PAGE_SIZE == TF_EM_PAGE_SIZE_1G)	/** 1G */
#define TF_EM_PAGE_SHIFT TF_EM_PAGE_SIZE_1G
#define TF_EM_PAGE_SIZE_ENUM HWRM_TF_CTXT_MEM_RGTR_INPUT_PAGE_SIZE_1G
#else
#error "Invalid Page Size specified. Please use a TF_EM_PAGE_SIZE_n define"
#endif

#define TF_EM_PAGE_SIZE	(1 << TF_EM_PAGE_SHIFT)
#define TF_EM_PAGE_ALIGNMENT (1 << TF_EM_PAGE_SHIFT)

/*
 * Used to build GFID:
 *
 *   15           2  0
 *  +--------------+--+
 *  |   Index      |E |
 *  +--------------+--+
 *
 * E = Entry (bucket inndex)
 */
#define TF_EM_INTERNAL_INDEX_SHIFT 2
#define TF_EM_INTERNAL_INDEX_MASK 0xFFFC
#define TF_EM_INTERNAL_ENTRY_MASK  0x3

/** EM Entry
 *  Each EM entry is 512-bit (64-bytes) but ordered differently to
 *  EEM.
 */
struct tf_em_64b_entry {
	/** Header is 8 bytes long */
	struct cfa_p4_eem_entry_hdr hdr;
	/** Key is 448 bits - 56 bytes */
	uint8_t key[TF_EM_KEY_RECORD_SIZE - sizeof(struct cfa_p4_eem_entry_hdr)];
};

/** EEM Memory Type
 *
 */
enum tf_mem_type {
	TF_EEM_MEM_TYPE_INVALID,
	TF_EEM_MEM_TYPE_HOST,
	TF_EEM_MEM_TYPE_SYSTEM
};

/**
 * tf_em_cfg_parms definition
 */
struct tf_em_cfg_parms {
	/**
	 * [in] Num entries in resource config
	 */
	uint16_t num_elements;
	/**
	 * [in] Resource config
	 */
	struct tf_rm_element_cfg *cfg;
	/**
	 * Session resource allocations
	 */
	struct tf_session_resources *resources;
	/**
	 * [in] Memory type.
	 */
	enum tf_mem_type mem_type;
};

/**
 * @page em EM
 *
 * @ref tf_alloc_eem_tbl_scope
 *
 * @ref tf_free_eem_tbl_scope_cb
 *
 * @ref tf_em_insert_int_entry
 *
 * @ref tf_em_delete_int_entry
 *
 * @ref tf_em_insert_ext_entry
 *
 * @ref tf_em_delete_ext_entry
 *
 * @ref tf_em_insert_ext_sys_entry
 *
 * @ref tf_em_delete_ext_sys_entry
 *
 * @ref tf_em_int_bind
 *
 * @ref tf_em_int_unbind
 *
 * @ref tf_em_ext_common_bind
 *
 * @ref tf_em_ext_common_unbind
 *
 * @ref tf_em_ext_host_alloc
 *
 * @ref tf_em_ext_host_free
 *
 * @ref tf_em_ext_system_alloc
 *
 * @ref tf_em_ext_system_free
 *
 * @ref tf_em_ext_common_free
 *
 * @ref tf_em_ext_common_alloc
 */

/**
 * Allocates EEM Table scope
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 *   -ENOMEM - Out of memory
 */
int tf_alloc_eem_tbl_scope(struct tf *tfp,
			   struct tf_alloc_tbl_scope_parms *parms);

/**
 * Free's EEM Table scope control block
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_free_eem_tbl_scope_cb(struct tf *tfp,
			     struct tf_free_tbl_scope_parms *parms);

/**
 * Insert record in to internal EM table
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_insert_int_entry(struct tf *tfp,
			   struct tf_insert_em_entry_parms *parms);

/**
 * Delete record from internal EM table
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_delete_int_entry(struct tf *tfp,
			   struct tf_delete_em_entry_parms *parms);

/**
 * Insert record in to external EEM table
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_insert_ext_entry(struct tf *tfp,
			   struct tf_insert_em_entry_parms *parms);

/**
 * Insert record from external EEM table
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_delete_ext_entry(struct tf *tfp,
			   struct tf_delete_em_entry_parms *parms);

/**
 * Insert record in to external system EEM table
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_insert_ext_sys_entry(struct tf *tfp,
			       struct tf_insert_em_entry_parms *parms);

/**
 * Delete record from external system EEM table
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_delete_ext_sys_entry(struct tf *tfp,
			       struct tf_delete_em_entry_parms *parms);

/**
 * Bind internal EM device interface
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_int_bind(struct tf *tfp,
		   struct tf_em_cfg_parms *parms);

/**
 * Unbind internal EM device interface
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_int_unbind(struct tf *tfp);

/**
 * Common bind for EEM device interface. Used for both host and
 * system memory
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_ext_common_bind(struct tf *tfp,
			  struct tf_em_cfg_parms *parms);

/**
 * Common unbind for EEM device interface. Used for both host and
 * system memory
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_ext_common_unbind(struct tf *tfp);

/**
 * Alloc for external EEM using host memory
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_ext_host_alloc(struct tf *tfp,
			 struct tf_alloc_tbl_scope_parms *parms);

/**
 * Free for external EEM using host memory
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_ext_host_free(struct tf *tfp,
			struct tf_free_tbl_scope_parms *parms);

/**
 * Alloc for external EEM using system memory
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_ext_system_alloc(struct tf *tfp,
			   struct tf_alloc_tbl_scope_parms *parms);

/**
 * Free for external EEM using system memory
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_ext_system_free(struct tf *tfp,
			  struct tf_free_tbl_scope_parms *parms);

/**
 * Common free for external EEM using host or system memory
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_ext_common_free(struct tf *tfp,
			  struct tf_free_tbl_scope_parms *parms);

/**
 * Common alloc for external EEM using host or system memory
 *
 * [in] tfp
 *   Pointer to TruFlow handle
 *
 * [in] parms
 *   Pointer to input parameters
 *
 * Returns:
 *   0       - Success
 *   -EINVAL - Parameter error
 */
int tf_em_ext_common_alloc(struct tf *tfp,
			   struct tf_alloc_tbl_scope_parms *parms);
#endif /* _TF_EM_H_ */
