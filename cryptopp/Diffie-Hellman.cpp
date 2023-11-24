// g++ -g3 -ggdb -O0 -I. -I/usr/include/cryptopp dh-xport.cpp -o dh-xport.exe -lcryptopp -lpthread
// g++ -g -O2 -I. -I/usr/include/cryptopp dh-xport.cpp -o dh-xport.exe -lcryptopp -lpthread

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <stdexcept>
using std::runtime_error;

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include "cryptopp/integer.h"
using CryptoPP::Integer;

#include "cryptopp/nbtheory.h"
using CryptoPP::ModularExponentiation;

#include "cryptopp/dh.h"
using CryptoPP::DH;

#include "cryptopp/dh2.h"
using CryptoPP::DH2;

#include "cryptopp/secblock.h"
using CryptoPP::SecByteBlock;

#include <cryptopp/cmac.h>
using CryptoPP::CMAC;

#include "cryptopp/aes.h"
using CryptoPP::AES;

#include "cryptopp/modes.h"
using CryptoPP::ECB_Mode;

int main(int argc, char** argv)
{
	try
	{
		// Generated offline using Crypto++.
		// 3072 bit safe prime.
		Integer p("0xbfef6a7e54ba72ba09873e7ad1f3dbac8e4ae5e55534974948d67c38e07baa8036f321b6372"
			"9469c02f70089426b6dcbf3b1f0c3574fc73f2509995e32b5a04e983d4363145f7ba599e760e85"
			"50991d21716f9b298a636b9a929c68a5ccc9af4f3cf1a8815002026df2853efbc28e9e1d7cc75b"
			"a53af6b618bed49b4d447100c4ddfd086d89f90a145ab912e7f335a595715eccb71409f3eaca38"
			"6db495d7c9e054001a514c2cb15c4438ca2e573defed0aaa76a4068bc1b2953094713a48f21168"
			"fbd30020fbe6e977e6d6908a25bd2983ff5e7e1237cf80b3b5f2f99dde22c63977718e073e7f2f"
			"44fea12d82ff53c031874958d6f6d81d156cc78b7a20ef8d31860f0dca106feacc7a2b609dd259"
			"3b1b1c1bc0d116a1ed2472f7507059f8108b0b6a3f69f3661e352c546c523ac85908e32f35155e"
			"9f4f0abbb70c83e122e51a4fee3184c45ecdfd50b5da59fefefc1f0d441074ed2269dc4b274e59"
			"1a8464d2c34f9c2e8cd1db308088503e9bbdf325f67b4830833ca10baae2fd1de7b57");

		Integer q = (p-1)/2;

		Integer g("0x2");

		// Safe primes are of the form p = 2q + 1, p and q prime.
		// These parameters do not state a maximum security level based
		// on the prime subgroup order. In essence, we get the maximum
		// security level. There is no free lunch: it means more modular
		// mutliplications are performed, which affects performance.

		// For a compare/contrast of meeting a security level, see dh-init.zip.
		// Also see http://www.cryptopp.com/wiki/Diffie-Hellman and
		// http://www.cryptopp.com/wiki/Security_level .

		DH dh;
		AutoSeededRandomPool rnd;

		dh.AccessGroupParameters().Initialize(p, g);

		if(!dh.GetGroupParameters().ValidateGroup(rnd, 3))
			throw runtime_error("Failed to validate prime and generator");

		size_t count = 0;

		p = dh.GetGroupParameters().GetModulus();
		q = dh.GetGroupParameters().GetSubgroupOrder();
		g = dh.GetGroupParameters().GetGenerator();

		//cout << "P: " << std::hex << p << endl;
		//cout << "Q: " << std::hex << q << endl;
		//cout << "G: " << std::hex << g << endl;

		// http://groups.google.com/group/sci.crypt/browse_thread/thread/7dc7eeb04a09f0ce
		Integer v = ModularExponentiation(g, q, p);
		if(v != Integer::One())
		{
			cout << "Verification: " << std::hex << v << endl;
			throw runtime_error("Failed to verify order of the subgroup");
		}

		//////////////////////////////////////////////////////////////

		DH2 dhA(dh), dhB(dh);

		SecByteBlock sprivA(dhA.StaticPrivateKeyLength()), spubA(dhA.StaticPublicKeyLength());
		SecByteBlock eprivA(dhA.EphemeralPrivateKeyLength()), epubA(dhA.EphemeralPublicKeyLength());

		SecByteBlock sprivB(dhB.StaticPrivateKeyLength()), spubB(dhB.StaticPublicKeyLength());
		SecByteBlock eprivB(dhB.EphemeralPrivateKeyLength()), epubB(dhB.EphemeralPublicKeyLength());

		dhA.GenerateStaticKeyPair(rnd, sprivA, spubA);
		dhA.GenerateEphemeralKeyPair(rnd, eprivA, epubA);

		dhB.GenerateStaticKeyPair(rnd, sprivB, spubB);
		dhB.GenerateEphemeralKeyPair(rnd, eprivB, epubB);

		//////////////////////////////////////////////////////////////

		if(dhA.AgreedValueLength() != dhB.AgreedValueLength())
			throw runtime_error("Shared secret size mismatch");

		SecByteBlock sharedA(dhA.AgreedValueLength()), sharedB(dhB.AgreedValueLength());

		if(!dhA.Agree(sharedA, sprivA, eprivA, spubB, epubB))
			throw runtime_error("Failed to reach shared secret (A)");

		if(!dhB.Agree(sharedB, sprivB, eprivB, spubA, epubA))
			throw runtime_error("Failed to reach shared secret (B)");

		count = std::min(dhA.AgreedValueLength(), dhB.AgreedValueLength());
		if(!count || 0 != memcmp(sharedA.BytePtr(), sharedB.BytePtr(), count))
			throw runtime_error("Failed to reach shared secret");

		//////////////////////////////////////////////////////////////

		// Take the leftmost 'n' bits for the KEK
		SecByteBlock kek(sharedA.BytePtr(), AES::DEFAULT_KEYLENGTH);

		// CMAC key follows the 'n' bits used for KEK
		SecByteBlock mack(&sharedA.BytePtr()[AES::DEFAULT_KEYLENGTH], AES::BLOCKSIZE);
		CMAC<AES> cmac(mack.BytePtr(), mack.SizeInBytes());

		// Generate a random CEK
		SecByteBlock cek(AES::DEFAULT_KEYLENGTH);
		rnd.GenerateBlock(cek.BytePtr(), cek.SizeInBytes());

		// AES in ECB mode is fine - we're encrypting 1 block, so we don't need padding
		ECB_Mode<AES>::Encryption aes;
		aes.SetKey(kek.BytePtr(), kek.SizeInBytes());

		// Will hold the encrypted key and cmac
		SecByteBlock xport(AES::BLOCKSIZE /*ENC(CEK)*/ + AES::BLOCKSIZE /*CMAC*/);
		CryptoPP::byte* const ptr = xport.BytePtr();

		// Write the encrypted key in the first 16 bytes, and the CMAC in the second 16 bytes
		// The logical layout of xport:
		//   [    Enc(CEK)    ][  CMAC(Enc(CEK))  ]
		aes.ProcessData(&ptr[0], cek.BytePtr(), AES::BLOCKSIZE);
		cmac.CalculateTruncatedDigest(&ptr[AES::BLOCKSIZE], AES::BLOCKSIZE, &ptr[0], AES::BLOCKSIZE);

		Integer a, b, c;

		a.Decode(kek.BytePtr(), kek.SizeInBytes());
		cout << "Key encryption key: " << std::hex << a << endl;

		b.Decode(cek.BytePtr(), cek.SizeInBytes());
		cout << "Content encryption key: " << std::hex << b << endl;

		c.Decode(xport.BytePtr(), xport.SizeInBytes());
		cout << "Encrypted CEK with CMAC: " << std::hex << c << endl;
	}

	catch(const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		return -2;
	}

	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		return -1;
	}

	return 0;
}

