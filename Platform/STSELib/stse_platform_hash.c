/******************************************************************************
 * \file    stse_platform_hash.c
 * \brief   STSecureElement hash platform for Linux (STM32MP1) using OpenSSL
 * \author  STMicroelectronics - CS application team
 *
 ******************************************************************************
 * \attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 STMicroelectronics</center></h2>
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "stse_conf.h"
#include "stselib.h"
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <string.h>

static const EVP_MD *stse_platform_get_openssl_md(stse_hash_algorithm_t hash_algo) {
    switch (hash_algo) {
#ifdef STSE_CONF_HASH_SHA_1
    case STSE_SHA_1:
        return EVP_sha1();
#endif
#ifdef STSE_CONF_HASH_SHA_224
    case STSE_SHA_224:
        return EVP_sha224();
#endif
#ifdef STSE_CONF_HASH_SHA_256
    case STSE_SHA_256:
        return EVP_sha256();
#endif
#ifdef STSE_CONF_HASH_SHA_384
    case STSE_SHA_384:
        return EVP_sha384();
#endif
#ifdef STSE_CONF_HASH_SHA_512
    case STSE_SHA_512:
        return EVP_sha512();
#endif
#ifdef STSE_CONF_HASH_SHA_3_256
    case STSE_SHA3_256:
        return EVP_sha3_256();
#endif
#ifdef STSE_CONF_HASH_SHA_3_384
    case STSE_SHA3_384:
        return EVP_sha3_384();
#endif
#ifdef STSE_CONF_HASH_SHA_3_512
    case STSE_SHA3_512:
        return EVP_sha3_512();
#endif
    default:
        return NULL;
    }
}

stse_ReturnCode_t stse_platform_hash_compute(stse_hash_algorithm_t hash_algo,
                                             PLAT_UI8 *pPayload, PLAT_UI16 payload_length,
                                             PLAT_UI8 *pHash, PLAT_UI16 *hash_length) {
    const EVP_MD *md;
    EVP_MD_CTX   *ctx;
    unsigned int  out_len;
    int           ret;

    md = stse_platform_get_openssl_md(hash_algo);
    if (md == NULL) {
        return STSE_PLATFORM_HASH_ERROR;
    }

    ctx = EVP_MD_CTX_new();
    if (ctx == NULL) {
        return STSE_PLATFORM_HASH_ERROR;
    }

    ret = EVP_DigestInit_ex(ctx, md, NULL);
    if (ret != 1) {
        EVP_MD_CTX_free(ctx);
        return STSE_PLATFORM_HASH_ERROR;
    }

    ret = EVP_DigestUpdate(ctx, pPayload, payload_length);
    if (ret != 1) {
        EVP_MD_CTX_free(ctx);
        return STSE_PLATFORM_HASH_ERROR;
    }

    out_len = (unsigned int)*hash_length;
    ret     = EVP_DigestFinal_ex(ctx, pHash, &out_len);
    EVP_MD_CTX_free(ctx);

    if (ret != 1) {
        return STSE_PLATFORM_HASH_ERROR;
    }

    *hash_length = (PLAT_UI16)out_len;

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_hmac_sha256_extract(PLAT_UI8 *pSalt, PLAT_UI16 salt_length,
                                                    PLAT_UI8 *pInput_keying_material, PLAT_UI16 input_keying_material_length,
                                                    PLAT_UI8 *pPseudorandom_key, PLAT_UI16 pseudorandom_key_expected_length) {
    unsigned int hmac_len = pseudorandom_key_expected_length;
    PLAT_UI8    *result;

    /* HKDF-Extract: PRK = HMAC-Hash(salt, IKM) */
    result = HMAC(EVP_sha256(),
                  pSalt, salt_length,
                  pInput_keying_material, input_keying_material_length,
                  pPseudorandom_key, &hmac_len);

    if (result == NULL || hmac_len != pseudorandom_key_expected_length) {
        return STSE_PLATFORM_HKDF_ERROR;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_hmac_sha256_expand(PLAT_UI8 *pPseudorandom_key, PLAT_UI16 pseudorandom_key_length,
                                                   PLAT_UI8 *pInfo, PLAT_UI16 info_length,
                                                   PLAT_UI8 *pOutput_keying_material, PLAT_UI16 output_keying_material_length) {
#define SHA256_DIGEST_LEN 32

    PLAT_UI8     tmp[SHA256_DIGEST_LEN];
    PLAT_UI16    tmp_length = 0;
    PLAT_UI16    out_index  = 0;
    PLAT_UI8     n          = 0x01;
    unsigned int hmac_len   = SHA256_DIGEST_LEN;

    if (pOutput_keying_material == NULL) {
        return STSE_PLATFORM_HKDF_ERROR;
    }

    /* RFC 5869: L <= 255*HashLen */
    if (((output_keying_material_length / SHA256_DIGEST_LEN) +
         ((output_keying_material_length % SHA256_DIGEST_LEN) != 0)) > 255) {
        return STSE_PLATFORM_HKDF_ERROR;
    }

    /* HKDF-Expand */
    while (out_index < output_keying_material_length) {
        PLAT_UI16 left = output_keying_material_length - out_index;
        HMAC_CTX *ctx  = HMAC_CTX_new();
        if (ctx == NULL) {
            return STSE_PLATFORM_HKDF_ERROR;
        }

        if (HMAC_Init_ex(ctx, pPseudorandom_key, pseudorandom_key_length, EVP_sha256(), NULL) != 1 ||
            HMAC_Update(ctx, tmp, tmp_length) != 1 ||
            HMAC_Update(ctx, pInfo, info_length) != 1 ||
            HMAC_Update(ctx, &n, 1) != 1 ||
            HMAC_Final(ctx, tmp, &hmac_len) != 1) {
            HMAC_CTX_free(ctx);
            memset(tmp, 0, SHA256_DIGEST_LEN);
            memset(pOutput_keying_material, 0, output_keying_material_length);
            return STSE_PLATFORM_HKDF_ERROR;
        }

        HMAC_CTX_free(ctx);

        left = (left < SHA256_DIGEST_LEN) ? left : SHA256_DIGEST_LEN;
        memcpy(pOutput_keying_material + out_index, tmp, left);

        tmp_length = SHA256_DIGEST_LEN;
        out_index += SHA256_DIGEST_LEN;
        n++;
    }

    memset(tmp, 0, SHA256_DIGEST_LEN);

    return STSE_OK;
}
