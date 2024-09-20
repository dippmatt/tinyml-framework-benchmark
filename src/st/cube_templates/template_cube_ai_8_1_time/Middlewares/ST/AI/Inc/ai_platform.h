/**
  ******************************************************************************
  * @file    ai_platform.h
  * @author  AST Embedded Analytics Research Platform
  * @brief   Definitions of AI platform public APIs types
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
  @endverbatim
  ******************************************************************************
  */

#ifndef AI_PLATFORM_H
#define AI_PLATFORM_H
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#ifndef AI_PLATFORM_API_MAJOR
#define AI_PLATFORM_API_MAJOR           (1)
#endif
#ifndef AI_PLATFORM_API_MINOR
#define AI_PLATFORM_API_MINOR           (2)
#endif
#ifndef AI_PLATFORM_API_MICRO
#define AI_PLATFORM_API_MICRO           (0)
#endif

#define AI_PLATFORM_API_VERSION \
  AI_VERSION(AI_PLATFORM_API_MAJOR, \
             AI_PLATFORM_API_MINOR, \
             AI_PLATFORM_API_MICRO)


#ifndef AI_TOOLS_API_VERSION_MAJOR
#define AI_TOOLS_API_VERSION_MAJOR      (1)
#endif
#ifndef AI_TOOLS_API_VERSION_MINOR
#define AI_TOOLS_API_VERSION_MINOR      (5)
#endif
#ifndef AI_TOOLS_API_VERSION_MICRO
#define AI_TOOLS_API_VERSION_MICRO      (0)
#endif

/*****************************************************************************/
#define AI_TOOLS_API_VERSION \
  AI_VERSION(AI_TOOLS_API_VERSION_MAJOR, \
             AI_TOOLS_API_VERSION_MINOR, \
             AI_TOOLS_API_VERSION_MICRO)

#define AI_TOOLS_API_VERSION_1_3 \
  AI_VERSION(1, 3, 0)

#define AI_TOOLS_API_VERSION_1_4 \
  AI_VERSION(1, 4, 0)

#define AI_TOOLS_API_VERSION_1_5 \
  AI_VERSION(1, 5, 0)

/*****************************************************************************/
#ifdef __cplusplus
#define AI_API_DECLARE_BEGIN extern "C" {
#define AI_API_DECLARE_END }
#else
#include <stdbool.h>
#define AI_API_DECLARE_BEGIN    /* AI_API_DECLARE_BEGIN */
#define AI_API_DECLARE_END      /* AI_API_DECLARE_END   */
#endif

/*****************************************************************************/
#define AI_FLAG_NONE            (0x0)

/*****************************************************************************/
AI_API_DECLARE_BEGIN

/*!
 * @typedef ai_flags
 * @ingroup ai_platform
 * @brief bitmask for flags management
 */
typedef uint32_t ai_flags;

/*****************************************************************************/
#define AI_CONCAT_ARG(a, b)     a ## b
#define AI_CONCAT(a, b)         AI_CONCAT_ARG(a, b)

/*!  AI_CAST SECTION                       ***********************************/
#define AI_CAST(type_, expr_)            ((type_)(expr_))

/*****************************************************************************/
#define AI_MAGIC_SIGNATURE \
  (0xa1facade)

#define AI_PACK(...) \
  __VA_ARGS__

/*****************************************************************************/
#define AI_SHAPE_BCWH             (0x01u)

/*!
 * @typedef ai_shape_dimension
 * @ingroup ai_platform
 * @brief shape dimension type to be used in shape related structs @ref ai_buffer_shape
 */
typedef uint32_t ai_shape_dimension;

/*****************************************************************************/
#if defined(_MSC_VER)
  #define AI_API_ENTRY          __declspec(dllexport)
  #define AI_ALIGNED(x)         /* AI_ALIGNED(x) */
#elif defined(__ICCARM__) || defined (__IAR_SYSTEMS_ICC__)
  #define AI_API_ENTRY          /* AI_API_ENTRY */
  #define AI_ALIGNED(x)         AI_CONCAT(AI_ALIGNED_,x)
  #define AI_ALIGNED_1          _Pragma("data_alignment = 1")
  #define AI_ALIGNED_2          _Pragma("data_alignment = 2")
  #define AI_ALIGNED_4          _Pragma("data_alignment = 4")
  #define AI_ALIGNED_8          _Pragma("data_alignment = 8")
  #define AI_ALIGNED_16         _Pragma("data_alignment = 16")
  #define AI_ALIGNED_32         _Pragma("data_alignment = 32")
#elif defined(__CC_ARM)
  #define AI_API_ENTRY          __attribute__((visibility("default")))
  #define AI_ALIGNED(x)         __attribute__((aligned (x)))
  /* Keil disallows anonymous union initialization by default */
  #pragma anon_unions
#elif defined(__GNUC__)
  //#define AI_API_ENTRY          __attribute__((visibility("default")))
  #define AI_API_ENTRY          /* AI_API_ENTRY */
  #define AI_ALIGNED(x)         __attribute__((aligned(x)))
#else
  /* Dynamic libraries are not supported by the compiler */
  #define AI_API_ENTRY                /* AI_API_ENTRY */
  #define AI_ALIGNED(x)               /* AI_ALIGNED(x) */
#endif

#define AI_HANDLE_PTR(ptr_)           ((ai_handle)(ptr_))
#define AI_HANDLE_NULL                AI_HANDLE_PTR(NULL)

#define AI_HANDLE_FUNC_PTR(func)      ((ai_handle_func)(func))

#define AI_UNUSED(x)                  (void)(x);

#define AI_DEPRECATED                 /* AI_DEPRECATED */

#define AI_LEGACY                     /* AI_LEGACY */

#define AI_MAGIC_MARKER               (0xA1FACADE)


#if defined(__cplusplus)
  #define AI_STRUCT_INIT              {}
  #define AI_C_ARRAY_INIT             {}
#else
  #define AI_STRUCT_INIT              {0}
  #define AI_C_ARRAY_INIT             {0}
#endif

#define AI_ERROR_FMT                  AIU32_FMT

#define AI_IS_UNSIGNED(type) \
  ((((type)0) - 1) > 0)

#define AI_CUSTOM_SIZE(type) \
  (ai_custom_type_signature)((AI_IS_UNSIGNED(type)) \
    ? (0x80|(sizeof(type)&0x7f)) : (sizeof(type)&0x7f))

/*! network buffers struct handlers *******************************************/
#ifdef __cplusplus

#define AI_NETWORK_PARAMS_INIT(params_, activations_) \
{ \
  {{ params_, activations_ }} \
}

#define AI_NETWORK_BUFFERS_INIT(weights_buffers_, activations_buffers_) \
{ \
  AI_MAGIC_SIGNATURE, AI_PACK(weights_buffers_), AI_PACK(activations_buffers_) \
}

#else

#define AI_NETWORK_PARAMS_INIT(params_, activations_) \
{ \
  .params = params_, \
  .activations = activations_ \
}

#define AI_NETWORK_BUFFERS_INIT(weights_buffers_, activations_buffers_) \
{ \
  .map_signature = AI_MAGIC_SIGNATURE, \
  .map_weights = AI_PACK(weights_buffers_), \
  .map_activations = AI_PACK(activations_buffers_) \
}

#endif    // __cplusplus


/*! binary padded bits macro helpers *****************************************/
#define AI_PBITS_MASK \
  (0x1F)

#define AI_PBITS_SHIFTS \
  (5)

#define AI_PBITS_PADDED_BYTES_COUNT(bits_) \
  (((ai_u32)(bits_) + 7) >> 3)

#define AI_PBITS_PADDED_WORDS_COUNT(bits_) \
  (((ai_size)(bits_) + AI_PBITS_MASK) >> AI_PBITS_SHIFTS)

#define AI_PBITS_GET_WORD(word_ptr_, bits_) \
  (((ai_pbits*)(word_ptr_)) + ((bits_) >> AI_PBITS_SHIFTS))

#define AI_PAD_CHANNELS(format_, channels_) \
  ((AI_BUFFER_FMT_GET_BITS(format_)==1) ? (AI_PBITS_PADDED_WORDS_COUNT(channels_) << AI_PBITS_SHIFTS) : (channels_))


/*! ai_intq_info struct handlers *********************************************/
#define INTQ_CONST    const
// #define INTQ_CONST

#define AI_INTQ_INFO_LIST(list_) \
  ((list_)->info)

#define AI_INTQ_INFO_LIST_FLAGS(list_) \
  ((list_) ? (list_)->flags : 0)

#define AI_INTQ_INFO_LIST_SIZE(list_) \
  ((list_) ? (list_)->size : 0)

#define AI_HAS_INTQ_INFO_LIST(list_) \
  ((list_) ? (((list_)->info) && ((list_)->size>0)) : false)

#define AI_INTQ_INFO_LIST_SCALE(list_, type_, pos_) \
  (((list_) && (list_)->info && ((pos_)<(list_)->size)) \
   ? ((type_*)((list_)->info->scale))[(pos_)] : 0)

#define AI_INTQ_INFO_LIST_ZEROPOINT(list_, type_, pos_) \
  (((list_) && (list_)->info && ((pos_)<(list_)->size)) \
   ? ((type_*)((list_)->info->zeropoint))[(pos_)] : 0)

/*! ai_buffer format handlers ************************************************/

/*!
 * @enum buffer format definition
 * @ingroup ai_platform
 *
 * 32 bit signed format list.
 */
typedef int32_t ai_buffer_format;

/*! ai_buffer_meta flags & macros ********************************************/
#define AI_BUFFER_META_HAS_INTQ_INFO        (0x1U << 0)
#define AI_BUFFER_META_FLAG_SCALE_FLOAT     (0x1U << 0)
#define AI_BUFFER_META_FLAG_ZEROPOINT_U8    (0x1U << 1)
#define AI_BUFFER_META_FLAG_ZEROPOINT_S8    (0x1U << 2)
#define AI_BUFFER_META_FLAG_ZEROPOINT_U16    (0x1U << 3)
#define AI_BUFFER_META_FLAG_ZEROPOINT_S16    (0x1U << 4)

/*! ai_buffer format variable flags & macros *********************************/
#define AI_BUFFER_FMT_TYPE_NONE          (0x0)
#define AI_BUFFER_FMT_TYPE_FLOAT         (0x1)
#define AI_BUFFER_FMT_TYPE_Q             (0x2)
#define AI_BUFFER_FMT_TYPE_BOOL          (0x3)

#define AI_BUFFER_FMT_FLAG_CONST         (0x1U<<30)
#define AI_BUFFER_FMT_FLAG_STATIC        (0x1U<<29)
#define AI_BUFFER_FMT_FLAG_IS_IO         (0x1U<<27)
#define AI_BUFFER_FMT_FLAG_PERSISTENT    (0x1U<<29)

#define AI_BUFFER_FMT_PACK(value_, mask_, bits_) \
  ( ((value_) & (mask_)) << (bits_) )

#define AI_BUFFER_FMT_UNPACK(fmt_, mask_, bits_) \
  ( (AI_BUFFER_FMT_OBJ(fmt_) >> (bits_)) & (mask_) )

#define AI_BUFFER_FMT_OBJ(fmt_) \
  ((ai_buffer_format)(fmt_))

#define AI_BUFFER_FMT_GET_FLOAT(fmt_) \
  AI_BUFFER_FMT_UNPACK(fmt_, 0x1, 24)

#define AI_BUFFER_FMT_GET_SIGN(fmt_) \
  AI_BUFFER_FMT_UNPACK(fmt_, 0x1, 23)

#define AI_BUFFER_FMT_GET_TYPE(fmt_) \
  AI_BUFFER_FMT_UNPACK(fmt_, 0xF, 17)

#define AI_BUFFER_FMT_GET_BITS(fmt_) \
  AI_BUFFER_FMT_UNPACK(fmt_, 0x7F, 7)

#define AI_BUFFER_FMT_SET_BITS(bits_) \
  AI_BUFFER_FMT_PACK((bits_), 0x7F, 7)

#define AI_BUFFER_FMT_GET_FBITS(fmt_) \
  ( (ai_i8)AI_BUFFER_FMT_UNPACK(fmt_, 0x7F, 0) - 64 )

#define AI_BUFFER_FMT_SET_FBITS(fbits_) \
  AI_BUFFER_FMT_PACK((fbits_)+64, 0x7F, 0)

#define AI_BUFFER_FMT_SET(type_id_, sign_bit_, float_bit_, bits_, fbits_) \
  AI_BUFFER_FMT_OBJ( \
    AI_BUFFER_FMT_PACK(float_bit_, 0x1, 24) | \
    AI_BUFFER_FMT_PACK(sign_bit_, 0x1, 23) | \
    AI_BUFFER_FMT_PACK(0, 0x3, 21) | \
    AI_BUFFER_FMT_PACK(type_id_, 0xF, 17) | \
    AI_BUFFER_FMT_PACK(0, 0x7, 14) | \
    AI_BUFFER_FMT_SET_BITS(bits_) | \
    AI_BUFFER_FMT_SET_FBITS(fbits_) \
  )

#define AI_BUFFER_FMT_SAME(fmt1_, fmt2_) \
  ( AI_BUFFER_FMT_GET(fmt1_) == AI_BUFFER_FMT_GET(fmt2_) )

#define AI_BUFFER_FMT_GET(fmt_) \
  (AI_BUFFER_FMT_OBJ(fmt_) & 0x01FFFFFF)

#define AI_BUFFER_FORMAT(buf_) \
  AI_BUFFER_FMT_GET((buf_)->format)


/*!
 * @define shape type index
 * @ingroup ai_platform
 * @brief positional ID for generic shapes C structs
 */
#define AI_SHAPE_EXTENSION      (0x5)
#define AI_SHAPE_DEPTH          (0x4)
#define AI_SHAPE_HEIGHT         (0x3)
#define AI_SHAPE_WIDTH          (0x2)
#define AI_SHAPE_CHANNEL        (0x1)
#define AI_SHAPE_IN_CHANNEL     (0x0)
#define AI_SHAPE_BATCH          (0x0)
#define AI_SHAPE_TIME           (0x0)


AI_DEPRECATED
#define AI_BUFFER_WIDTH(buf_) \
  ((buf_)->shape.data[AI_SHAPE_WIDTH])

AI_DEPRECATED
#define AI_BUFFER_HEIGHT(buf_) \
  ((buf_)->shape.data[AI_SHAPE_HEIGHT])

AI_DEPRECATED
#define AI_BUFFER_CHANNELS(buf_) \
  ((buf_)->shape.data[AI_SHAPE_CHANNEL])

AI_DEPRECATED
#define AI_BUFFER_N_BATCHES(buf_) \
  ((buf_)->shape.data[AI_SHAPE_BATCH])

#define AI_BUFFER_DATA(buf_, type_) \
  ((type_*)((buf_)->data))

#define AI_BUFFER_META_INFO(buf_) \
  ((buf_)->meta_info)

#define AI_BUFFER_META_INFO_INTQ(meta_) \
  ((meta_) && ((meta_)->flags & AI_BUFFER_META_HAS_INTQ_INFO)) \
    ? ((meta_)->intq_info) : NULL

#define AI_BUFFER_META_INFO_INTQ_GET_SIZE(meta_) \
  ( (AI_BUFFER_META_INFO_INTQ(meta_)) \
    ? AI_INTQ_INFO_LIST_SIZE(AI_BUFFER_META_INFO_INTQ(meta_)) \
    : 0 )

#define AI_BUFFER_META_INFO_INTQ_GET_SCALE(meta_, pos_) \
  ( (AI_BUFFER_META_INFO_INTQ(meta_)) \
    ? AI_INTQ_INFO_LIST_SCALE(AI_BUFFER_META_INFO_INTQ(meta_), ai_float, pos_) \
    : 0 )

#define AI_BUFFER_META_INFO_INTQ_GET_ZEROPOINT(meta_, pos_) \
  ( (AI_BUFFER_META_INFO_INTQ(meta_)) \
    ? ((AI_INTQ_INFO_LIST_FLAGS(AI_BUFFER_META_INFO_INTQ(meta_))&AI_BUFFER_META_FLAG_ZEROPOINT_U8) \
      ? AI_INTQ_INFO_LIST_ZEROPOINT(AI_BUFFER_META_INFO_INTQ(meta_), ai_u8, pos_) \
      : AI_INTQ_INFO_LIST_ZEROPOINT(AI_BUFFER_META_INFO_INTQ(meta_), ai_i8, pos_) ) \
    : 0 )

#define AI_BUFFER_META_INFO_INIT(flags_, intq_info_) { \
  .flags = (flags_), \
  .intq_info = AI_PACK(intq_info_) \
}

#define AI_BUFFER_SIZE(buf_) \
  ai_buffer_get_size(buf_, true)

#define AI_BUFFER_SIZE_UNPAD(buf_) \
  ai_buffer_get_size(buf_, false)

#define AI_BUFFER_BYTE_SIZE(count_, fmt_) \
  ai_buffer_get_byte_size(count_, fmt_)

#define AI_BUFFER_FLAGS(buf_) \
  ((buf_) ? (buf_)->flags : 0x0)

#define AI_BUFFER_SHAPE_INIT(type_, size_, ...) \
{ \
  .type = (type_), \
  .size  = (size_), \
  .data  = (ai_shape_dimension[]){ __VA_ARGS__ } \
}

#define AI_BUFFER_SHAPE_INIT_FROM_ARRAY(type_, size_, array_ptr_) \
{ \
  .type = (type_), \
  .size  = (size_), \
  .data  = (ai_shape_dimension*)(array_ptr_) \
}

#define AI_BUFFER_SHAPE_SIZE(buf_) \
  ((buf_) ? (buf_)->shape.size : 0)

#define AI_BUFFER_SHAPE_TYPE(buf_) \
  ((buf_) ? (buf_)->shape.type : 0)

#if defined(HAS_AI_ASSERT) && defined(AI_ASSERT)

#define AI_BUFFER_SET_SHAPE_ELEM(buf_, pos_, value_) { \
  AI_ASSERT(buf_) \
  (buf_)->shape.data[pos_] = (value_); \
}

#define AI_BUFFER_SHAPE_ELEM(buf_, pos_) \
  (((pos_)<AI_BUFFER_SHAPE_SIZE(buf_)) ? (buf_)->shape.data[pos_] : 0)

#else

#define AI_BUFFER_SET_SHAPE_ELEM(buf_, pos_, value_) { \
  (buf_)->shape.data[pos_] = (value_); \
}

#define AI_BUFFER_SHAPE_ELEM(buf_, pos_) \
  (buf_)->shape.data[pos_]

#endif


AI_DEPRECATED
#define AI_BUFFER_OBJ_INIT(format_, h_, w_, ch_, n_batches_, data_) \
{ .format = (ai_buffer_format)(format_), \
  .data = (ai_handle)(data_), \
  .meta_info = NULL, \
  .flags = AI_FLAG_NONE, \
  .size = (h_) * (w_) * AI_PAD_CHANNELS(format_, ch_), \
  .shape = AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, (n_batches_), (ch_), (w_), (h_)), \
}

AI_DEPRECATED
#define AI_BUFFER_OBJ_INIT_STATIC(type_, format_, h_, w_, ch_, n_batches_, ...) \
{ .format = (ai_buffer_format)(format_), \
  .data = (ai_handle)((type_[]){__VA_ARGS__}), \
  .meta_info = NULL, \
  .flags = AI_FLAG_NONE, \
  .size = (h_) * (w_) * AI_PAD_CHANNELS(format_, ch_), \
  .shape = AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, (n_batches_), (ch_), (w_), (h_)) \
}

/* 7.1 new macro API */
#define AI_BUFFER_INIT(flags_, format_, shape_, size_, meta_info_, data_) \
{ .format = (ai_buffer_format)(format_), \
  .data = (ai_handle)(data_), \
  .meta_info = (meta_info_), \
  .flags = (flags_), \
  .size = (size_), \
  .shape = AI_PACK(shape_) \
}

/* 7.1 new macro API */
#define AI_BUFFER_INIT_STATIC(type_, flags_, format_, shape_, size_, meta_info_, ...) \
{ .format = (ai_buffer_format)(format_), \
  .data = (ai_handle)((type_[]){__VA_ARGS__}), \
  .meta_info = (meta_info_), \
  .flags = (flags_), \
  .size = (size_), \
  .shape = AI_PACK(shape_) \
}

/*****************************************************************************/
#define AI_NETWORK_BUFFERS_FIELD_DECLARE \
  ai_signature      map_signature;      /*! structure signature (required!) */ \
  ai_buffer_array   map_weights;        /*! info about weights array buffers (required!) */ \
  ai_buffer_array   map_activations;    /*! info about activations array buffers (required!) */

#define AI_NETWORK_PARAMS_FIELDS_DECLARE \
union { \
  struct { \
    ai_buffer         params;         /*! info about params buffer(required!) */ \
    ai_buffer         activations;    /*! info about activations buffer (required!) */ \
  }; \
  struct { \
    AI_NETWORK_BUFFERS_FIELD_DECLARE \
  }; \
};

/*****************************************************************************/
#define AI_BUFFER_ARRAY_OBJ_INIT(flags_, size_, buffer_array_) \
{ \
  .flags = (ai_u16)(flags_), \
  .size = (ai_u16)(size_), \
  .buffer = (ai_buffer*)(buffer_array_) \
}

#define AI_BUFFER_ARRAY_OBJ_INIT_STATIC(flags_, size_, ...) \
{ \
  .flags = (ai_u16)(flags_), \
  .size = (ai_u16)(size_), \
  .buffer = (ai_buffer*)((ai_buffer[]){__VA_ARGS__}) \
}

#define AI_BUFFER_ARRAY_SANE(buf_array_) \
  ai_buffer_array_sane(buf_array_)

#define AI_BUFFER_ARRAY_FLAGS(buf_array_) \
  ((AI_BUFFER_ARRAY_SANE(buf_array_)) ? (buf_array_)->flags : AI_FLAG_NONE)

#define AI_BUFFER_ARRAY_SIZE(buf_array_) \
  ((AI_BUFFER_ARRAY_SANE(buf_array_)) ? (buf_array_)->size : 0)

#define AI_BUFFER_ARRAY_ITEM(buf_array_, pos_) \
  ((AI_BUFFER_ARRAY_SANE(buf_array_)) ? ((buf_array_)->buff