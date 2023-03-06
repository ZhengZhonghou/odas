   
   /**
    * \file     src_hops.c
    * \author   François Grondin <francois.grondin2@usherbrooke.ca>
    * \version  2.0
    * \date     2018-03-18
    * \copyright
    *
    * This program is free software: you can redistribute it and/or modify
    * it under the terms of the GNU General Public License as published by
    * the Free Software Foundation, either version 3 of the License, or
    * (at your option) any later version.
    *
    * This program is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    * 
    * You should have received a copy of the GNU General Public License
    * along with this program.  If not, see <http://www.gnu.org/licenses/>.
    *
    */
    
    #include <source/src_hops.h>

    src_hops_obj * src_hops_construct(const src_hops_cfg * src_hops_config, const msg_hops_cfg * msg_hops_config) {

        src_hops_obj * obj;
        unsigned int nBytes;

        obj = (src_hops_obj *) malloc(sizeof(src_hops_obj));

        obj->timeStamp = 0;

        obj->hopSize = msg_hops_config->hopSize;
        obj->nChannels = msg_hops_config->nChannels;
        obj->fS = msg_hops_config->fS;
        
        obj->format = format_clone(src_hops_config->format);
        obj->interface = interface_clone(src_hops_config->interface);

        memset(obj->bytes, 0x00, 4 * sizeof(char));

        if (!(((obj->interface->type == interface_file)  && (obj->format->type == format_binary_int08)) ||
              ((obj->interface->type == interface_file)  && (obj->format->type == format_binary_int16)) ||
              ((obj->interface->type == interface_file)  && (obj->format->type == format_binary_int24)) ||
              ((obj->interface->type == interface_file)  && (obj->format->type == format_binary_int32)) ||
              ((obj->interface->type == interface_soundcard)  && (obj->format->type == format_binary_int32)) ||
              ((obj->interface->type == interface_socket)  && (obj->format->type == format_binary_int16)) )) {
            
            printf("Source hops: Invalid interface and/or format.\n");
            exit(EXIT_FAILURE);

        }

        obj->buffer = (char *) malloc(sizeof(char) * obj->hopSize * obj->nChannels * 4);
        memset(obj->buffer, 0x00, sizeof(char) * obj->hopSize * obj->nChannels * 4);

        switch (obj->format->type) {

            case format_binary_int08: obj->bufferSize = obj->hopSize * obj->nChannels * 1; break;
            case format_binary_int16: obj->bufferSize = obj->hopSize * obj->nChannels * 2; break;
            case format_binary_int24: obj->bufferSize = obj->hopSize * obj->nChannels * 3; break;
            case format_binary_int32: obj->bufferSize = obj->hopSize * obj->nChannels * 4; break;

        }      

        obj->out = (msg_hops_obj *) NULL;
        
        return obj;

    }

    void src_hops_destroy(src_hops_obj * obj) {

        free((void *) obj->buffer);
        format_destroy(obj->format);
        interface_destroy(obj->interface);

        free((void *) obj);

    }

    void src_hops_connect(src_hops_obj * obj, msg_hops_obj * out) {

        obj->out = out;

    }

    void src_hops_disconnect(src_hops_obj * obj) {

        obj->out = (msg_hops_obj *) NULL;

    }

    void src_hops_open(src_hops_obj * obj) {

        switch(obj->interface->type) {

            case interface_file:

                src_hops_open_interface_file(obj);

            break;

            case interface_soundcard:

                src_hops_open_interface_soundcard(obj);

            break;

            case interface_socket:

                src_hops_open_interface_socket(obj);

            break;

            default:

                printf("Source hops: Invalid interface type.\n");
                exit(EXIT_FAILURE);

            break;

        }        

    }

    void src_hops_open_interface_file(src_hops_obj * obj) {

        obj->fp = fopen(obj->interface->fileName, "rb");

        if (obj->fp == NULL) {
            printf("Cannot open file %s\n",obj->interface->fileName);
            exit(EXIT_FAILURE);
        }

    }

    void src_hops_open_interface_soundcard(src_hops_obj * obj) {

    }

    void src_hops_open_interface_socket(src_hops_obj * obj) {

        memset(&(obj->sserver), 0x00, sizeof(struct sockaddr_in));

        obj->sserver.sin_family = AF_INET;
        obj->sserver.sin_addr.s_addr = htonl(INADDR_ANY);
        obj->sserver.sin_port = htons(obj->interface->port);
        obj->sid = socket(AF_INET, SOCK_DGRAM, 0);

        if ( bind(obj->sid , (struct sockaddr *) &(obj->sserver), sizeof(obj->sserver)) < 0 ) {

            printf("Sink pots: Cannot bind socket\n");
            exit(EXIT_FAILURE);

        }

    }

    void src_hops_close(src_hops_obj * obj) {

        switch(obj->interface->type) {

            case interface_file:

                src_hops_close_interface_file(obj);

            break;

            case interface_soundcard:

                src_hops_close_interface_soundcard(obj);

            break;

            case interface_socket:

                src_hops_close_interface_socket(obj);

            break;

            default:

                printf("Source hops: Invalid interface type.\n");
                exit(EXIT_FAILURE);

            break;

        }

    }

    void src_hops_close_interface_file(src_hops_obj * obj) {

        fclose(obj->fp);

    }

    void src_hops_close_interface_soundcard(src_hops_obj * obj) {

    }

    void src_hops_close_interface_socket(src_hops_obj * obj) {

        close(obj->sid);

    }

    int src_hops_process(src_hops_obj * obj) {

        int rtnValue;

        switch(obj->format->type) {

            case format_binary_int08:

                src_hops_process_format_binary_int08(obj);

            break;

            case format_binary_int16:

                src_hops_process_format_binary_int16(obj);                

            break;

            case format_binary_int24:

                src_hops_process_format_binary_int24(obj);

            break;

            case format_binary_int32:

                src_hops_process_format_binary_int32(obj);

            break;

            default:

                printf("Source hops: Invalid format type.\n");
                exit(EXIT_FAILURE);

            break;

        }

        switch(obj->interface->type) {

            case interface_file:

                rtnValue = src_hops_process_interface_file(obj);

            break;

            case interface_soundcard:

                rtnValue = src_hops_process_interface_soundcard(obj);

            break;

            case interface_socket:

                rtnValue = src_hops_process_interface_socket(obj);

            break;

            default:

                printf("Source hops: Invalid interface type.\n");
                exit(EXIT_FAILURE);

            break;           

        }

        obj->timeStamp++;
        obj->out->timeStamp = obj->timeStamp;

        return rtnValue;

    }

    int src_hops_process_interface_file(src_hops_obj * obj) {

        unsigned int nBytesTotal;
        int rtnValue;

        nBytesTotal = fread(obj->buffer, sizeof(char), obj->bufferSize, obj->fp);

        if (nBytesTotal == obj->bufferSize) {
            
            rtnValue = 0;    

        }
        else {

            rtnValue = -1;

        }

        return rtnValue;

    }

    int src_hops_process_interface_soundcard(src_hops_obj * obj) {
        int rtnValue = 0;
        return rtnValue;
    }

    int src_hops_process_interface_socket(src_hops_obj * obj) {

        char sbuffer[8192];
        int recv_len;

        char *buffer = obj->buffer;
        int remaining_size = obj->bufferSize;
        while (remaining_size > 0) {

            recv_len = recvfrom(obj->sid, sbuffer, sizeof(sbuffer), 0, NULL, 0);
            if (recv_len < 0) {
                printf("Socket error.\n");
                return -1;
            }

            memcpy(buffer, sbuffer, recv_len);
            remaining_size -= recv_len;
            buffer += recv_len;

        }

        //printf("Receive a frame.\n");
        return 0;

    }

    void src_hops_process_format_binary_int08(src_hops_obj * obj) {

        unsigned int iSample;
        unsigned int iChannel;
        unsigned int nBytes;
        float sample;

        nBytes = 1;

        for (iSample = 0; iSample < obj->hopSize; iSample++) {

            for (iChannel = 0; iChannel < obj->nChannels; iChannel++) {

                memcpy(&(obj->bytes[4-nBytes]),
                       &(obj->buffer[(iSample * obj->nChannels + iChannel) * nBytes]),
                       sizeof(char) * nBytes);

                sample = pcm_signedXXbits2normalized(obj->bytes, nBytes);

                obj->out->hops->array[iChannel][iSample] = sample;

            }

        }

    }

    void src_hops_process_format_binary_int16(src_hops_obj * obj) {

        unsigned int iSample;
        unsigned int iChannel;
        unsigned int nBytes;
        float sample;

        nBytes = 2;

        for (iSample = 0; iSample < obj->hopSize; iSample++) {

            for (iChannel = 0; iChannel < obj->nChannels; iChannel++) {

                memcpy(&(obj->bytes[4-nBytes]),
                       &(obj->buffer[(iSample * obj->nChannels + iChannel) * nBytes]),
                       sizeof(char) * nBytes);

                sample = pcm_signedXXbits2normalized(obj->bytes, nBytes);

                obj->out->hops->array[iChannel][iSample] = sample;

            }

        }

    }

    void src_hops_process_format_binary_int24(src_hops_obj * obj) {

        unsigned int iSample;
        unsigned int iChannel;
        unsigned int nBytes;
        float sample;

        nBytes = 3;

        for (iSample = 0; iSample < obj->hopSize; iSample++) {

            for (iChannel = 0; iChannel < obj->nChannels; iChannel++) {

                memcpy(&(obj->bytes[4-nBytes]),
                       &(obj->buffer[(iSample * obj->nChannels + iChannel) * nBytes]),
                       sizeof(char) * nBytes);

                sample = pcm_signedXXbits2normalized(obj->bytes, nBytes);

                obj->out->hops->array[iChannel][iSample] = sample;

            }

        }

    }

    void src_hops_process_format_binary_int32(src_hops_obj * obj) {

        unsigned int iSample;
        unsigned int iChannel;
        unsigned int nBytes;
        float sample;

        nBytes = 4;

        for (iSample = 0; iSample < obj->hopSize; iSample++) {

            for (iChannel = 0; iChannel < obj->nChannels; iChannel++) {

                memcpy(&(obj->bytes[4-nBytes]),
                       &(obj->buffer[(iSample * obj->nChannels + iChannel) * nBytes]),
                       sizeof(char) * nBytes);

                sample = pcm_signedXXbits2normalized(obj->bytes, nBytes);

                obj->out->hops->array[iChannel][iSample] = sample;

            }

        }

    }

    src_hops_cfg * src_hops_cfg_construct(void) {

        src_hops_cfg * cfg;

        cfg = (src_hops_cfg *) malloc(sizeof(src_hops_cfg));

        cfg->format = (format_obj *) NULL;
        cfg->interface = (interface_obj *) NULL;

        return cfg;

    }

    void src_hops_cfg_destroy(src_hops_cfg * src_hops_config) {

        if (src_hops_config->format != NULL) {
            format_destroy(src_hops_config->format);
        }
        if (src_hops_config->interface != NULL) {
            interface_destroy(src_hops_config->interface);
        }

        free((void *) src_hops_config);

    }

