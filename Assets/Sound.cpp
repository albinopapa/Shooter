/******************************************************************************************
 *	Chili DirectX Framework Version 12.10.21											  *
 *	Sound.cpp																			  *
 *	Copyright 2012 PlanetChili.net														  *
 *  Based on code obtained from http://www.rastertek.com								  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "Sound.h"
#include <assert.h>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib" )

DSound::DSound( HWND hWnd )
: pDirectSound( NULL ),
  pPrimaryBuffer( NULL )
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	result = DirectSoundCreate8( NULL,&pDirectSound,NULL );
	assert( !FAILED( result ) );

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = pDirectSound->SetCooperativeLevel( hWnd,DSSCL_PRIORITY );
	assert( !FAILED( result ) );

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = pDirectSound->CreateSoundBuffer( &bufferDesc,&pPrimaryBuffer,NULL );
	assert( !FAILED( result ) );

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified.
	result = pPrimaryBuffer->SetFormat( &waveFormat );
	assert( !FAILED( result ) );
}

DSound::~DSound()
{
	if( pPrimaryBuffer )
	{
		pPrimaryBuffer->Release();
		pPrimaryBuffer = NULL;
	}
	if( pDirectSound )
	{
		pDirectSound->Release();
		pDirectSound = NULL;
	}
}

// must be Stereo PCM Wave or IEEE float based Wave
Sound DSound::CreateSound( char* wavFileName )
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveRiffList waveFileHeader;
	WaveChunk waveChk;
	WAVEFORMATEX wfex={};
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	IDirectSoundBuffer8* pSecondaryBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;


	// Open the wave file in binary.
	error = fopen_s( &filePtr,wavFileName,"rb" );
	assert( error == 0 );

	// Read in the wave file header.
	count = fread( &waveFileHeader,sizeof( waveFileHeader ),1,filePtr );
	assert( count == 1 );

	// Check that the chunk ID is the RIFF format.
	assert(	(waveFileHeader.ckid[0] == 'R') &&
			(waveFileHeader.ckid[1] == 'I') &&
			(waveFileHeader.ckid[2] == 'F') &&
			(waveFileHeader.ckid[3] == 'F') );

	// Check that the file format is the WAVE format.
	assert(	(waveFileHeader.ckType[0] == 'W') &&
			(waveFileHeader.ckType[1] == 'A') &&
			(waveFileHeader.ckType[2] == 'V') &&
			(waveFileHeader.ckType[3] == 'E') );

	do {
		count = fread( &waveChk,sizeof( WaveChunk ),1,filePtr );
		assert( count == 1 );

		/* static int offset = 0;
		static WaveRiffList list = {};
		if( memcmp( waveChk.ckid, "LIST", 4 ) == 0 )
			fseek( filePtr, 4, SEEK_CUR );
			fread( &list, sizeof(WaveRiffList), 1, filePtr );
			offset += list.ckSize - 4;
			continue;
		} else */ if ( memcmp( waveChk.ckid, "fmt ", 4 ) != 0 ) {
			fseek( filePtr, waveChk.ckSize, SEEK_CUR );
			// offset -= (sizeof(waveChk)+waveChk.ckSize);
		}
	} while( memcmp( waveChk.ckid, "fmt ", 4 ) != 0 );

	// Check that the sub chunk ID is the fmt format.
	assert(	(waveChk.ckid[0] == 'f') &&
			(waveChk.ckid[1] == 'm') &&
			(waveChk.ckid[2] == 't') &&
			(waveChk.ckid[3] == ' ') );

	count = fread( &wfex, waveChk.ckSize, 1,filePtr );
	assert( count == 1 );

	// Check that the audio format is WAVE_FORMAT_PCM.
	assert( wfex.wFormatTag == WAVE_FORMAT_PCM || wfex.wFormatTag == WAVE_FORMAT_IEEE_FLOAT );

	// Check that the wave file was recorded in stereo format.
	assert( wfex.nChannels > 0 );

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	assert( wfex.nSamplesPerSec > 0 );

	// Ensure that the wave file was recorded in 16 bit format.
	assert( wfex.wBitsPerSample > 0 );

	do {
		count = fread( &waveChk,sizeof( WaveChunk ),1,filePtr );
		assert( count == 1 );

		/*static int offset = 0;
		static WaveRiffList list = {};
		if( memcmp( waveChk.ckid, "LIST", 4 ) == 0 )
			fseek( filePtr, 4, SEEK_CUR );
			fread( &list, sizeof(WaveRiffList), 1, filePtr );
			offset += list.ckSize - 4;
			continue;
		} else*/ if ( memcmp( waveChk.ckid, "data", 4 ) != 0 ) {
			fseek( filePtr, waveChk.ckSize, SEEK_CUR );
			//offset -= (sizeof(waveChk)+waveChk.ckSize);
		}
	} while( memcmp( waveChk.ckid, "data", 4 ) != 0 );

	// Check for the data chunk header.
	assert( (waveChk.ckid[0] == 'd') &&
			(waveChk.ckid[1] == 'a') &&
			(waveChk.ckid[2] == 't') &&
			(waveChk.ckid[3] == 'a') );

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS | DSBCAPS_STATIC;
	bufferDesc.dwBufferBytes = waveChk.ckSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &wfex;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	result = pDirectSound->CreateSoundBuffer( &bufferDesc,&tempBuffer,NULL );
	assert( !FAILED( result ) );

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface( IID_IDirectSoundBuffer8,(void**)&pSecondaryBuffer );
	assert( !FAILED( result ) );

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[ waveChk.ckSize ];
	assert( waveData );

	// Read in the wave file data into the newly created buffer.
	count = fread( waveData,1,waveChk.ckSize,filePtr );
	assert( count == waveChk.ckSize );

	// Close the file once done reading.
	error = fclose( filePtr );
	assert( error == 0 );

	// Lock the secondary buffer to write wave data into it.
	result = pSecondaryBuffer->Lock( 0,waveChk.ckSize,(void**)&bufferPtr,(DWORD*)&bufferSize,NULL,0,0 );
	assert( !FAILED( result ) );

	// Copy the wave data into the buffer.
	memcpy( bufferPtr,waveData,waveChk.ckSize );

	// Unlock the secondary buffer after the data has been written to it.
	result = pSecondaryBuffer->Unlock( (void*)bufferPtr,bufferSize,NULL,0 );
	assert( !FAILED( result ) );

	// Release the wave data since it was copied into the secondary buffer.
	delete [] waveData;
	waveData = NULL;

	return Sound( pSecondaryBuffer );
}

Sound::Sound( IDirectSoundBuffer8* pSecondaryBuffer )
: pBuffer( pSecondaryBuffer )
{}

Sound::Sound()
: pBuffer( NULL )
{}

Sound::Sound( const Sound& base )
: pBuffer( base.pBuffer )
{
	pBuffer->AddRef();
}

Sound::~Sound()
{
	if( pBuffer )
	{
		pBuffer->Release();
		pBuffer = NULL;
	}
}

const Sound& Sound::operator=( const Sound& rhs )
{
	this->~Sound();
	pBuffer = rhs.pBuffer;
	pBuffer->AddRef();
	return rhs;
}

// attn is the attenuation value in units of 0.01 dB (larger
// negative numbers give a quieter sound, 0 for full volume)
void Sound::Play( bool loop, int attn )
{
	attn = max( attn,DSBVOLUME_MIN );
	HRESULT result;

	// check that we have a valid buffer
	assert( pBuffer != NULL );

	// Set position at the beginning of the sound buffer.
	result = pBuffer->SetCurrentPosition( 0 );
	assert( !FAILED( result ) );

	// Set volume of the buffer to attn
	result = pBuffer->SetVolume( attn );
	assert( !FAILED( result ) );

	// Play the contents of the secondary sound buffer.
	result = pBuffer->Play( 0,0,loop?DSCBSTART_LOOPING:0 );
	assert( !FAILED( result ) );
}

void Sound::Stop()
{
	assert( pBuffer != NULL );
	pBuffer->Stop();
}

bool Sound::IsPlaying()
{
	DWORD dw;
	assert( pBuffer!= NULL );
	pBuffer->GetStatus( &dw );
	return dw & DSBSTATUS_PLAYING;
}
