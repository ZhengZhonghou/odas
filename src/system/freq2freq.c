    
    #include "freq2freq.h"

    freq2freq_obj * freq2freq_construct_zero(const unsigned int halfFrameSize, const unsigned int halfFrameSizeInterp, const unsigned int lowPassCut, const float epsilon, const float alpha, const float beta, const float Ginterf) {

        freq2freq_obj * obj;

        obj = (freq2freq_obj *) malloc(sizeof(freq2freq_obj));

        obj->halfFrameSize = halfFrameSize;
        obj->halfFrameSizeInterp = halfFrameSizeInterp;
        obj->lowPassCut = lowPassCut;
        obj->epsilon = epsilon;

        obj->alpha = alpha;
        obj->beta = beta;
        obj->Ginterf = Ginterf;

        return obj;

    }

    void freq2freq_destroy(freq2freq_obj * obj) {

        free((void *) obj);

    }

    void freq2freq_process_phasor(freq2freq_obj * obj, const freqs_obj * freqs, freqs_obj * phasors) {

        unsigned int iSignal, iSample;
        float real, imag;
        float magnitude;        

        for (iSignal = 0; iSignal < freqs->nSignals; iSignal++) {

            for (iSample = 0; iSample < freqs->halfFrameSize; iSample++) {

                real = freqs->array[iSignal][iSample * 2 + 0];
                imag = freqs->array[iSignal][iSample * 2 + 1];
                magnitude = sqrtf(real*real+imag*imag) + obj->epsilon;

                phasors->array[iSignal][iSample * 2 + 0] = real / magnitude;
                phasors->array[iSignal][iSample * 2 + 1] = imag / magnitude;

            }

        }

    }

    void freq2freq_process_product(freq2freq_obj * obj, const freqs_obj * freqs1, const freqs_obj * freqs2, const pairs_obj * pairs, freqs_obj * freqs12) {

        unsigned int iSignal1, iSignal2, iSignal12, iSample;
        float real1, imag1;
        float real2, imag2;

        iSignal12 = 0;

        for (iSignal1 = 0; iSignal1 < freqs1->nSignals; iSignal1++) {

            for (iSignal2 = (iSignal1+1); iSignal2 < freqs2->nSignals; iSignal2++) {

                if (pairs->array[iSignal12] == 0x01) {

                    for (iSample = 0; iSample < obj->halfFrameSize; iSample++) {

                        real1 = freqs1->array[iSignal1][iSample * 2 + 0];
                        imag1 = freqs1->array[iSignal1][iSample * 2 + 1];
                        real2 = freqs2->array[iSignal2][iSample * 2 + 0];
                        imag2 = freqs2->array[iSignal2][iSample * 2 + 1];

                        freqs12->array[iSignal12][iSample * 2 + 0] = real1 * real2 + imag1 * imag2;
                        freqs12->array[iSignal12][iSample * 2 + 1] = imag1 * real2 - imag2 * real1;                    

                    }

                }

                iSignal12++;

            }

        }

    }

    void freq2freq_process_lowpass(freq2freq_obj * obj, const freqs_obj * freqsAllPass, freqs_obj * freqsLowPass) {

        unsigned int iSignal;

        freqs_zero(freqsLowPass);

        for (iSignal = 0; iSignal < freqsAllPass->nSignals; iSignal++) {

            memcpy(freqsLowPass->array[iSignal], freqsAllPass->array[iSignal], sizeof(float) * 2 * obj->lowPassCut);               

        }

    }

    void freq2freq_process_interpolate(freq2freq_obj * obj, const freqs_obj * freqs, const freqs_obj * freqsInterp) {

        unsigned int iSignal;

        for (iSignal = 0; iSignal < freqs->nSignals; iSignal++) {

            memset(freqsInterp->array[iSignal], 0x00, sizeof(float) * 2 * obj->halfFrameSizeInterp);
            memcpy(freqsInterp->array[iSignal], freqs->array[iSignal], sizeof(float) * 2 * obj->halfFrameSize);

        }

    }