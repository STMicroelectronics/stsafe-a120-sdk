/******************************************************************************
 * \file    stse_platform_ecc.c
 * \brief   STSecureElement ECC platform for Linux (STM32MP1) using OpenSSL
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
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/ecdsa.h>
#include <openssl/evp.h>
#include <openssl/obj_mac.h>
#include <string.h>

/* --------------------------------------------------------------------------
 * Helper: get OpenSSL NID for a given STSE key type
 * -------------------------------------------------------------------------- */
static int stse_platform_get_nid(stse_ecc_key_type_t key_type) __attribute__((unused));
static int stse_platform_get_nid(stse_ecc_key_type_t key_type) {
    switch (key_type) {
#ifdef STSE_CONF_ECC_NIST_P_256
    case STSE_ECC_KT_NIST_P_256:
        return NID_X9_62_prime256v1;
#endif
#ifdef STSE_CONF_ECC_NIST_P_384
    case STSE_ECC_KT_NIST_P_384:
        return NID_secp384r1;
#endif
#ifdef STSE_CONF_ECC_NIST_P_521
    case STSE_ECC_KT_NIST_P_521:
        return NID_secp521r1;
#endif
#ifdef STSE_CONF_ECC_BRAINPOOL_P_256
    case STSE_ECC_KT_BP_P_256:
        return NID_brainpoolP256r1;
#endif
#ifdef STSE_CONF_ECC_BRAINPOOL_P_384
    case STSE_ECC_KT_BP_P_384:
        return NID_brainpoolP384r1;
#endif
#ifdef STSE_CONF_ECC_BRAINPOOL_P_512
    case STSE_ECC_KT_BP_P_512:
        return NID_brainpoolP512r1;
#endif
    default:
        return NID_undef;
    }
}

/* --------------------------------------------------------------------------
 * Helper: get coordinate size in bytes for a given STSE key type
 * -------------------------------------------------------------------------- */
static size_t stse_platform_get_coord_size(stse_ecc_key_type_t key_type) __attribute__((unused));
static size_t stse_platform_get_coord_size(stse_ecc_key_type_t key_type) {
    switch (key_type) {
#ifdef STSE_CONF_ECC_NIST_P_256
    case STSE_ECC_KT_NIST_P_256:
        return 32;
#endif
#ifdef STSE_CONF_ECC_NIST_P_384
    case STSE_ECC_KT_NIST_P_384:
        return 48;
#endif
#ifdef STSE_CONF_ECC_NIST_P_521
    case STSE_ECC_KT_NIST_P_521:
        return 66;
#endif
#ifdef STSE_CONF_ECC_BRAINPOOL_P_256
    case STSE_ECC_KT_BP_P_256:
        return 32;
#endif
#ifdef STSE_CONF_ECC_BRAINPOOL_P_384
    case STSE_ECC_KT_BP_P_384:
        return 48;
#endif
#ifdef STSE_CONF_ECC_BRAINPOOL_P_512
    case STSE_ECC_KT_BP_P_512:
        return 64;
#endif
#ifdef STSE_CONF_ECC_CURVE_25519
    case STSE_ECC_KT_CURVE25519:
        return 32;
#endif
#ifdef STSE_CONF_ECC_EDWARD_25519
    case STSE_ECC_KT_ED25519:
        return 32;
#endif
    default:
        return 0;
    }
}

/* --------------------------------------------------------------------------
 * ECDSA verify
 * STSE public key format: raw x || y (big-endian), no 0x04 prefix
 * STSE signature format: raw r || s (big-endian)
 * -------------------------------------------------------------------------- */
stse_ReturnCode_t stse_platform_ecc_verify(
    stse_ecc_key_type_t  key_type,
    const PLAT_UI8      *pPubKey,
    PLAT_UI8            *pDigest,
    PLAT_UI16            digestLen,
    PLAT_UI8            *pSignature) {

#ifdef STSE_CONF_ECC_EDWARD_25519
    if (key_type == STSE_ECC_KT_ED25519) {
        /* Ed25519 uses EVP interface with NULL digest */
        EVP_PKEY     *pkey = EVP_PKEY_new_raw_public_key(EVP_PKEY_ED25519, NULL, pPubKey, 32);
        EVP_MD_CTX   *ctx  = NULL;
        int           ret;

        if (pkey == NULL) {
            return STSE_PLATFORM_ECC_VERIFY_ERROR;
        }

        ctx = EVP_MD_CTX_new();
        if (ctx == NULL) {
            EVP_PKEY_free(pkey);
            return STSE_PLATFORM_ECC_VERIFY_ERROR;
        }

        /* Ed25519 signature = 64 bytes (r || s) */
        ret = EVP_DigestVerifyInit(ctx, NULL, NULL, NULL, pkey);
        if (ret == 1) {
            ret = EVP_DigestVerify(ctx, pSignature, 64, pDigest, digestLen);
        }

        EVP_MD_CTX_free(ctx);
        EVP_PKEY_free(pkey);

        return (ret == 1) ? STSE_OK : STSE_PLATFORM_ECC_VERIFY_ERROR;
    }
#endif /* STSE_CONF_ECC_EDWARD_25519 */

#if defined(STSE_CONF_ECC_NIST_P_256) || defined(STSE_CONF_ECC_NIST_P_384) || defined(STSE_CONF_ECC_NIST_P_521) || \
    defined(STSE_CONF_ECC_BRAINPOOL_P_256) || defined(STSE_CONF_ECC_BRAINPOOL_P_384) || defined(STSE_CONF_ECC_BRAINPOOL_P_512)
    {
        int                nid        = stse_platform_get_nid(key_type);
        size_t             coord_size = stse_platform_get_coord_size(key_type);
        EC_KEY            *ec_key     = NULL;
        EC_GROUP          *group      = NULL;
        EC_POINT          *pub_point  = NULL;
        ECDSA_SIG         *sig        = NULL;
        BIGNUM            *r          = NULL, *s = NULL;
        PLAT_UI8          *uncompressed_pub = NULL;
        stse_ReturnCode_t  ret             = STSE_PLATFORM_ECC_VERIFY_ERROR;

        if (nid == NID_undef || coord_size == 0) {
            return STSE_PLATFORM_ECC_VERIFY_ERROR;
        }

        group = EC_GROUP_new_by_curve_name(nid);
        if (group == NULL) goto ecc_verify_cleanup;

        ec_key = EC_KEY_new();
        if (ec_key == NULL) goto ecc_verify_cleanup;

        if (EC_KEY_set_group(ec_key, group) != 1) goto ecc_verify_cleanup;

        pub_point = EC_POINT_new(group);
        if (pub_point == NULL) goto ecc_verify_cleanup;

        /* Build uncompressed point: 0x04 || x || y */
        uncompressed_pub = (PLAT_UI8 *)malloc(1 + 2 * coord_size);
        if (uncompressed_pub == NULL) goto ecc_verify_cleanup;
        uncompressed_pub[0] = 0x04;
        memcpy(uncompressed_pub + 1, pPubKey, 2 * coord_size);

        if (EC_POINT_oct2point(group, pub_point, uncompressed_pub, 1 + 2 * coord_size, NULL) != 1) {
            goto ecc_verify_cleanup;
        }
        free(uncompressed_pub);
        uncompressed_pub = NULL;

        if (EC_KEY_set_public_key(ec_key, pub_point) != 1) goto ecc_verify_cleanup;

        /* Build ECDSA_SIG from raw r || s */
        r = BN_bin2bn(pSignature, (int)coord_size, NULL);
        s = BN_bin2bn(pSignature + coord_size, (int)coord_size, NULL);
        if (r == NULL || s == NULL) goto ecc_verify_cleanup;

        sig = ECDSA_SIG_new();
        if (sig == NULL) goto ecc_verify_cleanup;

        if (ECDSA_SIG_set0(sig, r, s) != 1) {
            BN_free(r);
            BN_free(s);
            r = NULL;
            s = NULL;
            goto ecc_verify_cleanup;
        }
        r = NULL; /* Ownership transferred to sig */
        s = NULL;

        if (ECDSA_do_verify(pDigest, (int)digestLen, sig, ec_key) == 1) {
            ret = STSE_OK;
        }

ecc_verify_cleanup:
        if (sig != NULL)             ECDSA_SIG_free(sig);
        if (r   != NULL)             BN_free(r);
        if (s   != NULL)             BN_free(s);
        if (pub_point != NULL)       EC_POINT_free(pub_point);
        if (ec_key != NULL)          EC_KEY_free(ec_key);
        if (group != NULL)           EC_GROUP_free(group);
        if (uncompressed_pub != NULL) free(uncompressed_pub);

        return ret;
    }
#else
    (void)key_type;
    (void)pPubKey;
    (void)pDigest;
    (void)digestLen;
    (void)pSignature;
    return STSE_PLATFORM_ECC_VERIFY_ERROR;
#endif
}

/* --------------------------------------------------------------------------
 * ECC key pair generation
 * -------------------------------------------------------------------------- */
stse_ReturnCode_t stse_platform_ecc_generate_key_pair(
    stse_ecc_key_type_t  key_type,
    PLAT_UI8            *pPrivKey,
    PLAT_UI8            *pPubKey) {

#ifdef STSE_CONF_ECC_EDWARD_25519
    if (key_type == STSE_ECC_KT_ED25519) {
        EVP_PKEY_CTX *ctx  = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);
        EVP_PKEY     *pkey = NULL;
        size_t        priv_len = 32, pub_len = 32;

        if (ctx == NULL) return STSE_PLATFORM_ECC_GENERATE_KEY_PAIR_ERROR;

        if (EVP_PKEY_keygen_init(ctx) != 1 ||
            EVP_PKEY_keygen(ctx, &pkey) != 1) {
            EVP_PKEY_CTX_free(ctx);
            return STSE_PLATFORM_ECC_GENERATE_KEY_PAIR_ERROR;
        }

        EVP_PKEY_CTX_free(ctx);

        if (EVP_PKEY_get_raw_private_key(pkey, pPrivKey, &priv_len) != 1 ||
            EVP_PKEY_get_raw_public_key(pkey, pPubKey, &pub_len) != 1) {
            EVP_PKEY_free(pkey);
            return STSE_PLATFORM_ECC_GENERATE_KEY_PAIR_ERROR;
        }

        EVP_PKEY_free(pkey);
        return STSE_OK;
    }
#endif /* STSE_CONF_ECC_EDWARD_25519 */

#ifdef STSE_CONF_ECC_CURVE_25519
    if (key_type == STSE_ECC_KT_CURVE25519) {
        EVP_PKEY_CTX *ctx  = EVP_PKEY_CTX_new_id(EVP_PKEY_X25519, NULL);
        EVP_PKEY     *pkey = NULL;
        size_t        priv_len = 32, pub_len = 32;

        if (ctx == NULL) return STSE_PLATFORM_ECC_GENERATE_KEY_PAIR_ERROR;

        if (EVP_PKEY_keygen_init(ctx) != 1 ||
            EVP_PKEY_keygen(ctx, &pkey) != 1) {
            EVP_PKEY_CTX_free(ctx);
            return STSE_PLATFORM_ECC_GENERATE_KEY_PAIR_ERROR;
        }

        EVP_PKEY_CTX_free(ctx);

        if (EVP_PKEY_get_raw_private_key(pkey, pPrivKey, &priv_len) != 1 ||
            EVP_PKEY_get_raw_public_key(pkey, pPubKey, &pub_len) != 1) {
            EVP_PKEY_free(pkey);
            return STSE_PLATFORM_ECC_GENERATE_KEY_PAIR_ERROR;
        }

        EVP_PKEY_free(pkey);
        return STSE_OK;
    }
#endif /* STSE_CONF_ECC_CURVE_25519 */

#if defined(STSE_CONF_ECC_NIST_P_256) || defined(STSE_CONF_ECC_NIST_P_384) || defined(STSE_CONF_ECC_NIST_P_521) || \
    defined(STSE_CONF_ECC_BRAINPOOL_P_256) || defined(STSE_CONF_ECC_BRAINPOOL_P_384) || defined(STSE_CONF_ECC_BRAINPOOL_P_512)
    {
        int                nid        = stse_platform_get_nid(key_type);
        size_t             coord_size = stse_platform_get_coord_size(key_type);
        EC_KEY            *ec_key     = NULL;
        const EC_POINT    *pub_point;
        const BIGNUM      *priv_bn;
        PLAT_UI8          *uncompressed = NULL;
        size_t             pub_len;
        stse_ReturnCode_t  ret         = STSE_PLATFORM_ECC_GENERATE_KEY_PAIR_ERROR;

        if (nid == NID_undef || coord_size == 0) {
            return STSE_PLATFORM_ECC_GENERATE_KEY_PAIR_ERROR;
        }

        ec_key = EC_KEY_new_by_curve_name(nid);
        if (ec_key == NULL) goto ecc_keygen_cleanup;

        if (EC_KEY_generate_key(ec_key) != 1) goto ecc_keygen_cleanup;

        /* Extract private key */
        priv_bn = EC_KEY_get0_private_key(ec_key);
        if (priv_bn == NULL) goto ecc_keygen_cleanup;
        if (BN_bn2binpad(priv_bn, pPrivKey, (int)coord_size) < 0) goto ecc_keygen_cleanup;

        /* Extract public key (x || y) */
        pub_point   = EC_KEY_get0_public_key(ec_key);
        pub_len     = 1 + 2 * coord_size;
        uncompressed = (PLAT_UI8 *)malloc(pub_len);
        if (uncompressed == NULL) goto ecc_keygen_cleanup;

        if (EC_POINT_point2oct(EC_KEY_get0_group(ec_key), pub_point,
                               POINT_CONVERSION_UNCOMPRESSED,
                               uncompressed, pub_len, NULL) != pub_len) {
            goto ecc_keygen_cleanup;
        }

        /* Copy x || y (skip 0x04 prefix) */
        memcpy(pPubKey, uncompressed + 1, 2 * coord_size);
        ret = STSE_OK;

ecc_keygen_cleanup:
        if (uncompressed != NULL) free(uncompressed);
        if (ec_key != NULL)       EC_KEY_free(ec_key);

        return ret;
    }
#else
    (void)key_type;
    (void)pPrivKey;
    (void)pPubKey;
    return STSE_PLATFORM_ECC_GENERATE_KEY_PAIR_ERROR;
#endif
}

/* --------------------------------------------------------------------------
 * ECDSA sign
 * -------------------------------------------------------------------------- */
#if defined(STSE_CONF_USE_HOST_KEY_PROVISIONING_WRAPPED_AUTHENTICATED) || \
    defined(STSE_CONF_USE_SYMMETRIC_KEY_ESTABLISHMENT_AUTHENTICATED) ||   \
    defined(STSE_CONF_USE_SYMMETRIC_KEY_PROVISIONING_WRAPPED_AUTHENTICATED)

stse_ReturnCode_t stse_platform_ecc_sign(
    stse_ecc_key_type_t  key_type,
    PLAT_UI8            *pPrivKey,
    PLAT_UI8            *pDigest,
    PLAT_UI16            digestLen,
    PLAT_UI8            *pSignature) {

    if (pPrivKey == NULL) {
        return STSE_PLATFORM_INVALID_PARAMETER;
    }

#ifdef STSE_CONF_ECC_EDWARD_25519
    if (key_type == STSE_ECC_KT_ED25519) {
        EVP_PKEY   *pkey = EVP_PKEY_new_raw_private_key(EVP_PKEY_ED25519, NULL, pPrivKey, 32);
        EVP_MD_CTX *ctx  = NULL;
        size_t      sig_len = 64;
        int         ret;

        if (pkey == NULL) return STSE_PLATFORM_ECC_SIGN_ERROR;

        ctx = EVP_MD_CTX_new();
        if (ctx == NULL) {
            EVP_PKEY_free(pkey);
            return STSE_PLATFORM_ECC_SIGN_ERROR;
        }

        ret = EVP_DigestSignInit(ctx, NULL, NULL, NULL, pkey);
        if (ret == 1) {
            ret = EVP_DigestSign(ctx, pSignature, &sig_len, pDigest, digestLen);
        }

        EVP_MD_CTX_free(ctx);
        EVP_PKEY_free(pkey);

        return (ret == 1) ? STSE_OK : STSE_PLATFORM_ECC_SIGN_ERROR;
    }
#endif /* STSE_CONF_ECC_EDWARD_25519 */

#if defined(STSE_CONF_ECC_NIST_P_256) || defined(STSE_CONF_ECC_NIST_P_384) || defined(STSE_CONF_ECC_NIST_P_521) || \
    defined(STSE_CONF_ECC_BRAINPOOL_P_256) || defined(STSE_CONF_ECC_BRAINPOOL_P_384) || defined(STSE_CONF_ECC_BRAINPOOL_P_512)
    {
        int                nid        = stse_platform_get_nid(key_type);
        size_t             coord_size = stse_platform_get_coord_size(key_type);
        EC_KEY            *ec_key     = NULL;
        ECDSA_SIG         *sig        = NULL;
        BIGNUM            *priv_bn    = NULL;
        const BIGNUM      *r, *s;
        stse_ReturnCode_t  ret        = STSE_PLATFORM_ECC_SIGN_ERROR;

        if (nid == NID_undef || coord_size == 0) {
            return STSE_PLATFORM_ECC_SIGN_ERROR;
        }

        ec_key = EC_KEY_new_by_curve_name(nid);
        if (ec_key == NULL) goto ecc_sign_cleanup;

        priv_bn = BN_bin2bn(pPrivKey, (int)coord_size, NULL);
        if (priv_bn == NULL) goto ecc_sign_cleanup;

        if (EC_KEY_set_private_key(ec_key, priv_bn) != 1) goto ecc_sign_cleanup;

        sig = ECDSA_do_sign(pDigest, (int)digestLen, ec_key);
        if (sig == NULL) goto ecc_sign_cleanup;

        ECDSA_SIG_get0(sig, &r, &s);

        if (BN_bn2binpad(r, pSignature, (int)coord_size) < 0) goto ecc_sign_cleanup;
        if (BN_bn2binpad(s, pSignature + coord_size, (int)coord_size) < 0) goto ecc_sign_cleanup;

        ret = STSE_OK;

ecc_sign_cleanup:
        if (sig != NULL)     ECDSA_SIG_free(sig);
        if (priv_bn != NULL) BN_free(priv_bn);
        if (ec_key != NULL)  EC_KEY_free(ec_key);

        return ret;
    }
#else
    (void)key_type;
    (void)pPrivKey;
    (void)pDigest;
    (void)digestLen;
    (void)pSignature;
    return STSE_PLATFORM_ECC_SIGN_ERROR;
#endif
}

#endif /* STSE_CONF_USE_HOST_KEY_PROVISIONING_WRAPPED_AUTHENTICATED || ... */

/* --------------------------------------------------------------------------
 * ECDH key agreement
 * -------------------------------------------------------------------------- */
#if defined(STSE_CONF_USE_HOST_KEY_ESTABLISHMENT) ||                      \
    defined(STSE_CONF_USE_HOST_KEY_PROVISIONING_WRAPPED) ||               \
    defined(STSE_CONF_USE_HOST_KEY_PROVISIONING_WRAPPED_AUTHENTICATED) || \
    defined(STSE_CONF_USE_SYMMETRIC_KEY_ESTABLISHMENT) ||                 \
    defined(STSE_CONF_USE_SYMMETRIC_KEY_ESTABLISHMENT_AUTHENTICATED) ||   \
    defined(STSE_CONF_USE_SYMMETRIC_KEY_PROVISIONING_WRAPPED) ||          \
    defined(STSE_CONF_USE_SYMMETRIC_KEY_PROVISIONING_WRAPPED_AUTHENTICATED)

stse_ReturnCode_t stse_platform_ecc_ecdh(
    stse_ecc_key_type_t  key_type,
    const PLAT_UI8      *pPubKey,
    const PLAT_UI8      *pPrivKey,
    PLAT_UI8            *pSharedSecret) {

#ifdef STSE_CONF_ECC_CURVE_25519
    if (key_type == STSE_ECC_KT_CURVE25519) {
        EVP_PKEY     *local_key  = EVP_PKEY_new_raw_private_key(EVP_PKEY_X25519, NULL, pPrivKey, 32);
        EVP_PKEY     *remote_key = EVP_PKEY_new_raw_public_key(EVP_PKEY_X25519, NULL, pPubKey, 32);
        EVP_PKEY_CTX *ctx        = NULL;
        size_t        secret_len = 32;
        int           ret_val;

        if (local_key == NULL || remote_key == NULL) {
            if (local_key != NULL)  EVP_PKEY_free(local_key);
            if (remote_key != NULL) EVP_PKEY_free(remote_key);
            return STSE_PLATFORM_ECC_ECDH_ERROR;
        }

        ctx     = EVP_PKEY_CTX_new(local_key, NULL);
        ret_val = (ctx != NULL &&
                   EVP_PKEY_derive_init(ctx) == 1 &&
                   EVP_PKEY_derive_set_peer(ctx, remote_key) == 1 &&
                   EVP_PKEY_derive(ctx, pSharedSecret, &secret_len) == 1) ? 1 : 0;

        if (ctx != NULL)        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(local_key);
        EVP_PKEY_free(remote_key);

        return (ret_val == 1) ? STSE_OK : STSE_PLATFORM_ECC_ECDH_ERROR;
    }
#endif /* STSE_CONF_ECC_CURVE_25519 */

#if defined(STSE_CONF_ECC_NIST_P_256) || defined(STSE_CONF_ECC_NIST_P_384) || defined(STSE_CONF_ECC_NIST_P_521) || \
    defined(STSE_CONF_ECC_BRAINPOOL_P_256) || defined(STSE_CONF_ECC_BRAINPOOL_P_384) || defined(STSE_CONF_ECC_BRAINPOOL_P_512)
    {
        int                nid           = stse_platform_get_nid(key_type);
        size_t             coord_size    = stse_platform_get_coord_size(key_type);
        EC_KEY            *local_key     = NULL;
        EC_KEY            *remote_key    = NULL;
        EC_GROUP          *group         = NULL;
        EC_POINT          *pub_point     = NULL;
        BIGNUM            *priv_bn       = NULL;
        PLAT_UI8          *uncompressed  = NULL;
        stse_ReturnCode_t  ret           = STSE_PLATFORM_ECC_ECDH_ERROR;
        int                secret_len;

        if (nid == NID_undef || coord_size == 0) {
            return STSE_PLATFORM_ECC_ECDH_ERROR;
        }

        group = EC_GROUP_new_by_curve_name(nid);
        if (group == NULL) goto ecdh_cleanup;

        /* Build local (private) key */
        local_key = EC_KEY_new_by_curve_name(nid);
        if (local_key == NULL) goto ecdh_cleanup;

        priv_bn = BN_bin2bn(pPrivKey, (int)coord_size, NULL);
        if (priv_bn == NULL) goto ecdh_cleanup;

        if (EC_KEY_set_private_key(local_key, priv_bn) != 1) goto ecdh_cleanup;

        /* Build remote (public) key */
        remote_key = EC_KEY_new_by_curve_name(nid);
        if (remote_key == NULL) goto ecdh_cleanup;

        pub_point   = EC_POINT_new(group);
        if (pub_point == NULL) goto ecdh_cleanup;

        uncompressed = (PLAT_UI8 *)malloc(1 + 2 * coord_size);
        if (uncompressed == NULL) goto ecdh_cleanup;
        uncompressed[0] = 0x04;
        memcpy(uncompressed + 1, pPubKey, 2 * coord_size);

        if (EC_POINT_oct2point(group, pub_point, uncompressed, 1 + 2 * coord_size, NULL) != 1) {
            goto ecdh_cleanup;
        }

        if (EC_KEY_set_public_key(remote_key, pub_point) != 1) goto ecdh_cleanup;

        /* Compute shared secret (x-coordinate only) */
        secret_len = ECDH_compute_key(pSharedSecret, coord_size, pub_point, local_key, NULL);
        if (secret_len > 0) {
            ret = STSE_OK;
        }

ecdh_cleanup:
        if (uncompressed != NULL) free(uncompressed);
        if (pub_point != NULL)    EC_POINT_free(pub_point);
        if (priv_bn != NULL)      BN_free(priv_bn);
        if (local_key != NULL)    EC_KEY_free(local_key);
        if (remote_key != NULL)   EC_KEY_free(remote_key);
        if (group != NULL)        EC_GROUP_free(group);

        return ret;
    }
#else
    (void)key_type;
    (void)pPubKey;
    (void)pPrivKey;
    (void)pSharedSecret;
    return STSE_PLATFORM_ECC_ECDH_ERROR;
#endif
}

#endif /* STSE_CONF_USE_HOST_KEY_ESTABLISHMENT || ... */

/* --------------------------------------------------------------------------
 * NIST AES Key Wrap (RFC 3394)
 * -------------------------------------------------------------------------- */
#if defined(STSE_CONF_USE_HOST_KEY_PROVISIONING_WRAPPED) ||               \
    defined(STSE_CONF_USE_HOST_KEY_PROVISIONING_WRAPPED_AUTHENTICATED) || \
    defined(STSE_CONF_USE_SYMMETRIC_KEY_PROVISIONING_WRAPPED) ||          \
    defined(STSE_CONF_USE_SYMMETRIC_KEY_PROVISIONING_WRAPPED_AUTHENTICATED)

#include <openssl/aes.h>

#define KEK_WRAP_IV_SIZE 8
static const PLAT_UI8 KEK_WRAP_IV[KEK_WRAP_IV_SIZE] = {0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6};

stse_ReturnCode_t stse_platform_nist_kw_encrypt(PLAT_UI8 *pPayload, PLAT_UI32 payload_length,
                                                PLAT_UI8 *pKey, PLAT_UI8 key_length,
                                                PLAT_UI8 *pOutput, PLAT_UI32 *pOutput_length) {
    AES_KEY aes_key;
    int     wrapped_len;

    if (AES_set_encrypt_key(pKey, key_length * 8, &aes_key) != 0) {
        return STSE_PLATFORM_KEYWRAP_ERROR;
    }

    wrapped_len = AES_wrap_key(&aes_key,
                               KEK_WRAP_IV,
                               pOutput,
                               pPayload,
                               (unsigned int)payload_length);

    if (wrapped_len <= 0) {
        return STSE_PLATFORM_KEYWRAP_ERROR;
    }

    *pOutput_length = (PLAT_UI32)wrapped_len;

    return STSE_OK;
}

#endif /* STSE_CONF_USE_HOST_KEY_PROVISIONING_WRAPPED || ... */
