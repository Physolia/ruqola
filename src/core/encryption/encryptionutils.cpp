/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "encryptionutils.h"
#include "ruqola_encryption_debug.h"
#include <QByteArray>
// https://docs.rocket.chat/customer-center/security-center/end-to-end-encryption-specifications

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>

using namespace Qt::Literals::StringLiterals;
QByteArray EncryptionUtils::exportJWKKey(RSA *rsaKey)
{
    const BIGNUM *n, *e, *d;
    RSA_get0_key(rsaKey, &n, &e, &d);

    if (!n || !e) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << " Impossible to get RSA";
        return {};
    }

    // Convert BIGNUM to base64url strings
    QByteArray nBytes(BN_num_bytes(n), 0);
    BN_bn2bin(n, reinterpret_cast<unsigned char *>(nBytes.data()));
    const QString nBase64Url = QString::fromLatin1(nBytes.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));

    QByteArray eBytes(BN_num_bytes(e), 0);
    BN_bn2bin(e, reinterpret_cast<unsigned char *>(eBytes.data()));
    const QString eBase64Url = QString::fromLatin1(eBytes.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));

    QJsonObject jwkObj;
    jwkObj["kty"_L1] = "RSA"_L1;
    jwkObj["n"_L1] = nBase64Url;
    jwkObj["e"_L1] = eBase64Url;

    QJsonDocument doc(jwkObj);
    return doc.toJson(QJsonDocument::Compact);
}

void EncryptionUtils::generateRSAKey()
{
    int ret = 0;
    RSA *rsa = nullptr;
    BIGNUM *bne = nullptr;
    BIO *bp_public = nullptr;
    BIO *bp_private = nullptr;

    int bits = 2048;
    unsigned long e = RSA_F4; // équivalent à 0x10001

    bne = BN_new();
    ret = BN_set_word(bne, e);
    if (ret != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error when generating exponent";
        return;
    }

    rsa = RSA_new();
    ret = RSA_generate_key_ex(rsa, bits, bne, nullptr);
    if (ret != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error during generate key";
        return;
    }

    bp_public = BIO_new_file("public_key.pem", "w+");
    ret = PEM_write_bio_RSAPublicKey(bp_public, rsa);
    if (ret != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error when saving public key";
        return;
    }

    bp_private = BIO_new_file("private_key.pem", "w+");
    ret = PEM_write_bio_RSAPrivateKey(bp_private, rsa, nullptr, nullptr, 0, nullptr, nullptr);
    if (ret != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error when saving private key";
        return;
    }

    // Libérer la mémoire
    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
    RSA_free(rsa);
    BN_free(bne);
}

QString EncryptionUtils::encodePrivateKey(const QString &privateKey, const QString &password, const QString &userId)
{
    const QString masterKey = getMasterKey(password, userId);
    return {};
}

QString EncryptionUtils::deriveKey(const QVector<uint8_t> &, const QByteArray &ba)
{
#if 0
    export async function deriveKey(salt, baseKey, keyUsages = ['encrypt', 'decrypt']) {
            const iterations = 1000;
            const hash = 'SHA-256';

            return crypto.subtle.deriveKey({ name: 'PBKDF2', salt, iterations, hash }, baseKey, { name: 'AES-CBC', length: 256 }, true, keyUsages);
    }

#endif
    const int iterations = 1000;
    const QByteArray hash = "SHA-256";
    // TODO
    return {};
}

QString EncryptionUtils::getMasterKey(const QString &password, const QString &userId)
{
    if (password.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Password can't be null. It's a bug";
        return {};
    }

#if 0
    // First, create a PBKDF2 "key" containing the password
    const QByteArray baseKey = importRawKey(toArrayBuffer(password.toUtf8()));
    if (baseKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Problem during import raw key";
        return {};
    }
    // Derive a key from the password
    return deriveKey(toArrayBuffer(userId.toLatin1()), baseKey);
#endif
#if 0
    async getMasterKey(password: string): Promise<void | CryptoKey> {
            if (password == null) {
                    alert('You should provide a password');
            }

            // First, create a PBKDF2 "key" containing the password
            let baseKey;
            try {
                    baseKey = await importRawKey(toArrayBuffer(password));
            } catch (error) {
                    this.setState(E2EEState.ERROR);
                    return this.error('Error creating a key based on user password: ', error);
            }

            // Derive a key from the password
            try {
                    return await deriveKey(toArrayBuffer(Meteor.userId()), baseKey);
            } catch (error) {
                    this.setState(E2EEState.ERROR);
                    return this.error('Error deriving baseKey: ', error);
            }
    }

#endif

    // TODO
    return {};
}

QByteArray EncryptionUtils::encryptAES_CBC(const QByteArray &data, const QByteArray &key, const QByteArray &iv)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;
    unsigned char ciphertext[128];

    if (!(ctx = EVP_CIPHER_CTX_new()))
        return {};

    if (1
        != EVP_EncryptInit_ex(ctx,
                              EVP_aes_256_cbc(),
                              NULL,
                              reinterpret_cast<const unsigned char *>(key.data()),
                              reinterpret_cast<const unsigned char *>(iv.data())))
        return {};

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, reinterpret_cast<const unsigned char *>(data.data()), data.size()))
        return {};
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        return {};
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return QByteArray(reinterpret_cast<char *>(ciphertext), ciphertext_len);
}

QByteArray EncryptionUtils::generateRandomIV(int size)
{
    QByteArray iv(size, 0);
    // TODO QRandomGenerator::global()->generate(reinterpret_cast<quint8*>(iv.data()), size);
    return iv;
}

QByteArray EncryptionUtils::deriveKey(const QByteArray &keyData, const QByteArray &salt, int iterations)
{
    unsigned char key[32]; // 256-bit key
    if (!PKCS5_PBKDF2_HMAC(keyData.data(),
                           keyData.size(),
                           reinterpret_cast<const unsigned char *>(salt.data()),
                           salt.size(),
                           iterations,
                           EVP_sha256(),
                           32,
                           key)) {
        return {};
    }
    return QByteArray(reinterpret_cast<char *>(key), 32);
}

EncryptionUtils::EncryptionInfo EncryptionUtils::splitVectorAndEcryptedData(const QByteArray &cipherText)
{
    EncryptionUtils::EncryptionInfo info;
    if (!cipherText.isEmpty()) {
        // TODO add more check
        info.vector = cipherText.left(16);
        info.encryptedData = cipherText.last(16);
    }
    return info;
}

QByteArray EncryptionUtils::joinVectorAndEcryptedData(const EncryptionUtils::EncryptionInfo &info)
{
    return info.vector + info.encryptedData;
}

QVector<uint8_t> EncryptionUtils::toArrayBuffer(const QByteArray &ba)
{
    const QVector<uint8_t> byteVector(ba.constBegin(), ba.constEnd());
    return byteVector;
}

// return crypto.subtle.importKey(
//         'jwk',
//         keyData,
//         {
//                 name: 'RSA-OAEP',
//                 modulusLength: 2048,
//                 publicExponent: new Uint8Array([0x01, 0x00, 0x01]),
//                 hash: { name: 'SHA-256' },
//         },
//         true,
//         keyUsages,
// );
void EncryptionUtils::importRSAKey()
{
    // TODO
}

// return crypto.subtle.importKey('jwk', keyData, { name: 'AES-CBC' }, true, keyUsages);
void EncryptionUtils::importAESKey()
{
#if 0
    export async function importAESKey(keyData, keyUsages = ['encrypt', 'decrypt']) {
            return crypto.subtle.importKey('jwk', keyData, { name: 'AES-CBC' }, true, keyUsages);
    }

#endif

    // TODO
}

// crypto.subtle.importKey('raw', keyData, { name: 'PBKDF2' }, false, keyUsages);
QByteArray EncryptionUtils::importRawKey(const QByteArray &keyData, const QByteArray &salt, int iterations)
{
#if 0
    export async function importRawKey(keyData, keyUsages = ['deriveKey']) {
            return crypto.subtle.importKey('raw', keyData, { name: 'PBKDF2' }, false, keyUsages);
    }

#endif

    // TODO
    QByteArray iv;
    QByteArray plainText;

    const QByteArray key = deriveKey(keyData, salt, iterations);
    const QByteArray cipherText = encryptAES_CBC(plainText, key, iv);
    return cipherText;
}

bool EncryptionUtils::EncryptionInfo::isValid() const
{
    return !vector.isEmpty() && !encryptedData.isEmpty();
}

bool EncryptionUtils::EncryptionInfo::operator==(const EncryptionUtils::EncryptionInfo &other) const
{
    return other.vector == vector && other.encryptedData == encryptedData;
}

QString EncryptionUtils::generateRandomPassword()
{
    const int numberChar = 30;
    QString randomStr;
    for (int i = 0; i < numberChar; i++) {
        const int d = rand() % 200; // Generate a random ASCII value between 0 and 199
        if (d >= 33 && d <= 123) {
            randomStr.append(QLatin1Char(static_cast<char>(d))); // Convert the ASCII value to a character for valid range
        } else {
            randomStr.append(QString::number(d % 10)); // Keep the last digit for numbers outside the valid range
        }
    }
    return randomStr;
}
