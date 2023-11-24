// Driver.cpp
//

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif

#ifdef _DEBUG
# pragma comment( lib, "cryptlibd" )
#else
# pragma comment( lib, "cryptlib" )
#endif


#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include <iostream>
using std::endl;
using std::cout;
using std::cerr;

#include <string>
using std::string;

#include "cryptopp/cryptlib.h"
using CryptoPP::Exception;

#include "cryptopp/files.h"
using CryptoPP::FileSink;
using CryptoPP::FileSource;

#include "cryptopp/filters.h"
using CryptoPP::ArraySink;
using CryptoPP::StringSource;
using CryptoPP::StringSink;
using CryptoPP::StringStore;
using CryptoPP::Redirector;

using CryptoPP::SignerFilter;
using CryptoPP::SignatureVerificationFilter;

#include "cryptopp/dsa.h"
using CryptoPP::DSA;

#include "assert.h"

void SaveKey( const DSA::PrivateKey& PrivateKey, const string& filename );
void LoadKey( const string& filename, DSA::PrivateKey& PrivateKey );

int main(int argc, char* argv[])
{
    try
    {
        AutoSeededRandomPool rng;

        // Generate Private Key
        DSA::PrivateKey PrivateKey;
        PrivateKey.GenerateRandomWithKeySize(rng, 1024);
        if (!PrivateKey.Validate(rng, 3))
        {
            throw("DSA key generation failed");
        }
        SaveKey( PrivateKey, "key.pv.bin" );
        //LoadKey( "key.pv.bin", PrivateKey );

        // Generate Public Key
        DSA::PublicKey PublicKey;
        PublicKey.AssignFrom(PrivateKey);

        // Sign and verify
        string message = "DSA Signature";
        string signature;

        DSA::Signer signer( PrivateKey );
        StringSource( message, true,
            new SignerFilter( rng, signer,
                new StringSink( signature )
            ) // SignerFilter
        ); // StringSource

        // Tamper
        {
            // signature[0] |= 0x0F;
            // message[0] |= 0x0F;
        }

        DSA::Verifier verifier( PublicKey );
        StringSource( message+signature, true,
            new SignatureVerificationFilter(
                verifier, NULL,
                SignatureVerificationFilter::THROW_EXCEPTION
            )
        );

        cout << "Verified signature on message" << endl;

    }
    catch( SignatureVerificationFilter::SignatureVerificationFailed& e )
    {
        cerr << "caught SignatureVerificationFailed..." << endl;
        cerr << e.what() << endl;
    }
    catch( CryptoPP::Exception& e )
    {
        cerr << "caught Exception..." << endl;
        cerr << e.what() << endl;
    }

    return 0;
}

void SaveKey( const DSA::PublicKey& PublicKey, const string& filename )
{
    // DER Encode Key - X.509 key format
    PublicKey.Save(
        FileSink( filename.c_str(), true /*binary*/ ).Ref()
    );
}

void SaveKey( const DSA::PrivateKey& PrivateKey, const string& filename )
{
    // DER Encode Key - PKCS #8 key format
    PrivateKey.Save(
        FileSink( filename.c_str(), true /*binary*/ ).Ref()
    );
}

void LoadKey( const string& filename, DSA::PublicKey& PublicKey )
{
    // DER Encode Key - X.509 key format
    PublicKey.Load(
        FileSource( filename.c_str(), true, NULL, true /*binary*/ ).Ref()
    );
}

void LoadKey( const string& filename, DSA::PrivateKey& PrivateKey )
{
    // DER Encode Key - PKCS #8 key format
    PrivateKey.Load(
        FileSource( filename.c_str(), true, NULL, true /*binary*/ ).Ref()
    );
}

void VerifyMessageThrow1( const DSA::PublicKey& PublicKey,
    const string& message, const string& signature )
{
    DSA::Verifier verifier( PublicKey );

    StringSource( signature+message, true,
        new SignatureVerificationFilter(
            verifier, NULL,
            SignatureVerificationFilter::THROW_EXCEPTION |
            SignatureVerificationFilter::SIGNATURE_AT_BEGIN
        )
    );
}

void VerifyMessageThrow2( const DSA::PublicKey& PublicKey,
    const string& message, const string& signature )
{
    DSA::Verifier verifier( PublicKey );

    StringSource( message+signature, true,
        new SignatureVerificationFilter(
            verifier, NULL,
            SignatureVerificationFilter::THROW_EXCEPTION
        )
    );
}

void VerifyMessageNoThrow1(const DSA::PublicKey& PublicKey,
    const string& message, const string& signature )
{
    DSA::Verifier verifier( PublicKey );

    bool result = false;
    StringSource( message+signature, true,
        new SignatureVerificationFilter(
            verifier,
            new ArraySink( (CryptoPP::byte*)&result, sizeof(result ) ),
            SignatureVerificationFilter::PUT_RESULT
        )
    );
}

void VerifyMessageNoThrow2(const DSA::PublicKey& PublicKey,
    const string& message, const string& signature )
{
    DSA::Verifier verifier( PublicKey );

    SignatureVerificationFilter svf(
        verifier
    ); // SignatureVerificationFilter

    StringSource( signature+message, true,
        new Redirector( svf )
    ); // StringSource

    bool b = svf.GetLastResult();
}
