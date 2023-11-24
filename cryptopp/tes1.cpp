

// stdafx.h : menyertakan file untuk sistem standar termasuk file,
// atau proyek tertentu menyertakan file yang sering digunakan, namun
// jarang diubah
//

#pragma once

#pragma once

// Makro berikut menentukan platform minimum yang diperlukan. Platform minimum yang diperlukan
// adalah versi paling awal dari Windows, Internet Explorer, dll. yang memiliki fitur yang diperlukan untuk dijalankan
// aplikasi Anda. Makro bekerja dengan mengaktifkan semua fitur yang tersedia pada versi platform hingga dan
// termasuk versi yang ditentukan.

// Ubah definisi berikut jika Anda harus menargetkan platform sebelum yang ditentukan di bawah.
// Lihat MSDN untuk info terbaru tentang nilai yang sesuai untuk platform berbeda.
#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0500     // Change this to the appropriate value to target other versions of Windows.
#endif



#include <windows.h>
#include <tchar.h>

#include <iostream>
using std::cin;
using std::wcin;
using std::cout;
using std::wcout;
using std::cerr;
using std::wcerr;
using std::endl;

#include <string>
using std::string;
using std::wstring;

#ifdef UNICODE
#  define tcin wcin
#  define tcout wcout
#  define tcerr wcerr
#  define tstring wstring
#else
#  define tcin cin
#  define tcout cout
#  define tcerr cerr
#  define tstring string
#endif



#include <iostream>
using std::cout;
using std::cerr;

#include <string>
using std::string;

#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include "cryptopp/cryptlib.h"
using CryptoPP::BufferedTransformation;
using CryptoPP::AuthenticatedSymmetricCipher;

#include "cryptopp/filters.h"
using CryptoPP::Redirector;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::AuthenticatedEncryptionFilter;
using CryptoPP::AuthenticatedDecryptionFilter;

#include "cryptopp/aes.h"
using CryptoPP::AES;

#include "cryptopp/ccm.h"
using CryptoPP::CCM;

#include "assert.h"

int main(int argc, char* argv[])
{
    // The test vectors use both ADATA and PDATA. However,
    //  as a drop in replacement for older modes such as
    //  CBC, we only exercise (and need) plain text.

    AutoSeededRandomPool prng;

    byte key[ AES::DEFAULT_KEYLENGTH ];
    prng.GenerateBlock( key, sizeof(key) );

    // { 7, 8, 9, 10, 11, 12, 13 }
    byte iv[ 12 ];
    prng.GenerateBlock( iv, sizeof(iv) );

    // { 4, 6, 8, 10, 12, 14, 16 }
    const int TAG_SIZE = 8;

    string pdata="Authenticated Encryption";

    // Encrypted, with Tag
    string cipher, encoded;

    // Recovered
    string rpdata;

    /*********************************\
    \*********************************/

    // Pretty print
    encoded.clear();
    StringSource( key, sizeof(key), true,
        new HexEncoder(
            new StringSink( encoded )
        ) // HexEncoder
    ); // StringSource
    cout << "key: " << encoded << endl;

    // Pretty print
    encoded.clear();
    StringSource( iv, sizeof(iv), true,
        new HexEncoder(
            new StringSink( encoded )
        ) // HexEncoder
    ); // StringSource
    cout << " iv: " << encoded << endl;

    cout << endl;

    /*********************************\
    \*********************************/

    try
    {
        cout << "plain text: " << pdata << endl;

        CCM< AES, TAG_SIZE >::Encryption e;
        e.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
        e.SpecifyDataLengths( 0, pdata.size(), 0 );

        StringSource( pdata, true,
            new AuthenticatedEncryptionFilter( e,
                new StringSink( cipher )
            ) // AuthenticatedEncryptionFilter
        ); // StringSource
    }
    catch( CryptoPP::InvalidArgument& e )
    {
        cerr << "Caught InvalidArgument..." << endl;
        cerr << e.what() << endl;
        cerr << endl;
    }
    catch( CryptoPP::Exception& e )
    {
        cerr << "Caught Exception..." << endl;
        cerr << e.what() << endl;
        cerr << endl;
    }

    /*********************************\
    \*********************************/

    // Pretty print
    encoded.clear();
    StringSource( cipher, true,
        new HexEncoder(
            new StringSink( encoded )
        ) // HexEncoder
    ); // StringSource
    cout << "cipher text: " << encoded << endl;

    // Attack the first and last byte
    //if( cipher.size() > 1 )
    //{
    // cipher[ 0 ] |= 0x0F;
    // cipher[ cipher.size()-1 ] |= 0x0F;
    //}

    /*********************************\
    \*********************************/

    try
    {
        CCM< AES, TAG_SIZE >::Decryption d;
        d.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
        d.SpecifyDataLengths( 0, cipher.size()-TAG_SIZE, 0 );

        AuthenticatedDecryptionFilter df( d,
            new StringSink( rpdata )
        ); // AuthenticatedDecryptionFilter

        // Dtor StringSource akan segera dipanggil
        // setelah konstruksi di bawah. Hal ini akan menyebabkan
        // penghancuran benda yang dimilikinya. Untuk menghentikan
        // perilaku sehingga kita bisa mendapatkan hasil decodingnya
        // DecryptionFilter, kita harus menggunakan redirector
        // atau secara manual Masukkan(...) ke dalam filter tanpa
        // menggunakan StringSource.
        StringSource( cipher, true,
            new Redirector( df /*, PASS_EVERYTHING */ )
        ); // StringSource

        // If the object does not throw, here's the only
        //  opportunity to check the data's integrity
        bool b = df.GetLastResult();
        assert( true == b );

        cout << "recovered text: " << rpdata << endl;
    }
    catch( CryptoPP::HashVerificationFilter::HashVerificationFailed& e )
    {
        cerr << "Caught HashVerificationFailed..." << endl;
        cerr << e.what() << endl;
        cerr << endl;
    }
    catch( CryptoPP::InvalidArgument& e )
    {
        cerr << "Caught InvalidArgument..." << endl;
        cerr << e.what() << endl;
        cerr << endl;
    }
    catch( CryptoPP::Exception& e )
    {
        cerr << "Caught Exception..." << endl;
        cerr << e.what() << endl;
        cerr << endl;
    }

    /*********************************\
    \*********************************/

    return 0;
}
