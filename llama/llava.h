/**
 * llama.cpp - git d5c938cd7716b9a2ace49a43a469dfbffcff4d28
 *
 * MIT License
 *
 * Copyright (c) 2023-2024 The ggml authors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LLAVA_H
#define LLAVA_H

#include "ggml.h"

#ifdef LLAMA_SHARED
#    if defined(_WIN32) && !defined(__MINGW32__)
#        ifdef LLAMA_BUILD
#            define LLAVA_API __declspec(dllexport)
#        else
#            define LLAVA_API __declspec(dllimport)
#        endif
#    else
#        define LLAVA_API __attribute__ ((visibility ("default")))
#    endif
#else
#    define LLAVA_API
#endif

struct clip_ctx;

#ifdef __cplusplus
extern "C" {
#endif

struct llava_image_embed {
    float * embed;
    int n_image_pos;
};

/** sanity check for clip <-> llava embed size match */
LLAVA_API bool llava_validate_embed_size(const struct llama_context * ctx_llama, const struct clip_ctx * ctx_clip);

LLAVA_API bool llava_image_embed_make_with_clip_img(struct clip_ctx * ctx_clip, int n_threads, const struct clip_image_u8 * img, float ** image_embd_out, int * n_img_pos_out);

/** build an image embed from image file bytes */
LLAVA_API struct llava_image_embed * llava_image_embed_make_with_bytes(struct clip_ctx * ctx_clip, int n_threads, const unsigned char * image_bytes, int image_bytes_length);
/** build an image embed from a path to an image filename */
LLAVA_API struct llava_image_embed * llava_image_embed_make_with_filename(struct clip_ctx * ctx_clip, int n_threads, const char * image_path);
LLAVA_API void llava_image_embed_free(struct llava_image_embed * embed);
/** free an embedding made with llava_image_embed_make_* */

/** write the image represented by embed into the llama context with batch size n_batch, starting at context pos n_past. on completion, n_past points to the next position in the context after the image embed. */
LLAVA_API bool llava_eval_image_embed(struct llama_context * ctx_llama, const struct llava_image_embed * embed, int n_batch, int * n_past);

#ifdef __cplusplus
}
#endif

#endif
