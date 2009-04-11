/*******************************************************************************
*                                                                              *
*   SDL_ffmpeg is a library for basic multimedia functionality.                *
*   SDL_ffmpeg is based on ffmpeg.                                             *
*                                                                              *
*   Copyright (C) 2007  Arjan Houben                                           *
*                                                                              *
*   SDL_ffmpeg is free software: you can redistribute it and/or modify         *
*   it under the terms of the GNU Lesser General Public License as published   *
*	by the Free Software Foundation, either version 3 of the License, or any   *
*   later version.                                                             *
*                                                                              *
*   This program is distributed in the hope that it will be useful,            *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of             *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               *
*   GNU Lesser General Public License for more details.                        *
*                                                                              *
*   You should have received a copy of the GNU Lesser General Public License   *
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.      *
*                                                                              *
*******************************************************************************/

#include "SDL/SDL_ffmpeg.h"

#include <string.h>

int main(int argc, char** argv) {

    SDL_ffmpegFile      *audioFile;

    /* check if we got an argument */
    if( argc < 2 ) {
        printf( "usage: \"%s\" \"filename\"\n", argv[0] );
        return -1;
    }

    /* open file from arg[1] */
    audioFile = SDL_ffmpegOpen( argv[1] );
    if( !audioFile ) {
        printf( "error opening file\n" );
        return -1;
    }

    /* select the stream you want to decode (example just uses 0 as a default) */
    SDL_ffmpegSelectAudioStream( audioFile, 0 );

    /* create an audio frame to store data received from SDL_ffmpegGetAudioFrame */
    SDL_ffmpegAudioFrame *frame = SDL_ffmpegCreateAudioFrame( audioFile, 32 );
    if( !frame ) {
        printf( "couldn't prepare frame buffer\n" );
        return -1;
    }

    SDL_ffmpegGetAudioFrame( audioFile, frame );

    FILE *f = fopen("test.raw","wb");

    while( !frame->last ) {
        fwrite( frame->buffer, 1, frame->size, f );
        SDL_ffmpegGetAudioFrame( audioFile, frame );
    }


    SDL_ffmpegPacket **p = &audioFile->audioStream->buffer;
int i=0;
    while( *p ) {
        p = &(*p)->next;
        i++;
    }
    printf("pakcets: %i\n",i);

    fclose( f );

    SDL_ffmpegFreeAudio( frame );

    /* when we are done with the file, we free it */
    SDL_ffmpegFree( audioFile );

    /* the SDL_Quit function offcourse... */
    SDL_Quit();

    return 0;
}
