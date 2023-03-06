#
# This file is the odaslive recipe.
#

SUMMARY = "Simple odaslive application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://include/ \
    file://src/general/format.c \
    file://src/general/interface.c \
    file://src/general/link.c \
    file://src/general/mic.c \
    file://src/general/samplerate.c \
    file://src/general/soundspeed.c \
    file://src/general/spatialfilter.c \    
    file://src/general/thread.c \
    file://src/init/combining.c \
    file://src/init/delay.c \
    file://src/init/directivity.c \
    file://src/init/hit.c \
    file://src/init/linking.c \
    file://src/init/scanning.c \
    file://src/init/space.c \
    file://src/init/windowing.c \
    file://src/signal/acorr.c \
    file://src/signal/aimg.c \
    file://src/signal/area.c \
    file://src/signal/assignation.c \
    file://src/signal/beampattern.c \
    file://src/signal/category.c \
    file://src/signal/coherence.c \
    file://src/signal/delta.c \
    file://src/signal/demixing.c \
    file://src/signal/env.c \
    file://src/signal/frame.c \
    file://src/signal/freq.c \
    file://src/signal/gain.c \
    file://src/signal/hop.c \
    file://src/signal/index.c \
    file://src/signal/kalman.c \
    file://src/signal/map.c \
    file://src/signal/mask.c \
    file://src/signal/mixture.c \
    file://src/signal/pair.c \
    file://src/signal/particle.c \
    file://src/signal/pitch.c \
    file://src/signal/point.c \
    file://src/signal/postprob.c \
    file://src/signal/pot.c \
    file://src/signal/scan.c \
    file://src/signal/spatialgain.c \
    file://src/signal/spatialindex.c \
    file://src/signal/spatialmask.c \
    file://src/signal/steer.c \
    file://src/signal/target.c \
    file://src/signal/tau.c \
    file://src/signal/tdoa.c \
    file://src/signal/track.c \
    file://src/signal/triangle.c \
    file://src/signal/window.c \
    file://src/signal/xcorr.c \
    file://src/system/acorr2pitch.c \
    file://src/system/demixing2env.c \
    file://src/system/demixing2freq.c \
    file://src/system/env2env.c \
    file://src/system/hop2hop.c \
    file://src/system/hop2frame.c \
    file://src/system/frame2freq.c \
    file://src/system/freq2acorr.c \
    file://src/system/freq2env.c \
    file://src/system/freq2freq.c \
    file://src/system/freq2frame.c \
    file://src/system/freq2xcorr.c \
    file://src/system/frame2hop.c \
    file://src/system/gain2mask.c \
    file://src/system/kalman2coherence.c \
    file://src/system/kalman2kalman.c \
    file://src/system/mixture2mixture.c \
    file://src/system/particle2coherence.c \
    file://src/system/particle2particle.c \
    file://src/system/pitch2category.c \
    file://src/system/steer2demixing.c \
    file://src/system/track2gain.c \
    file://src/system/track2steer.c \
    file://src/system/xcorr2aimg.c \
    file://src/system/xcorr2xcorr.c \
    file://src/utils/cmatrix.c \
    file://src/utils/fft.c \
    file://src/utils/fifo.c \
    file://src/utils/gaussian.c \
    file://src/utils/gaussjordan.c \
    file://src/utils/inverse.c \
    file://src/utils/matrix.c \
    file://src/utils/pcm.c \
    file://src/utils/random.c \
    file://src/utils/transcendental.c \
    file://src/aconnector/acon_categories.c \
    file://src/aconnector/acon_hops.c \
    file://src/aconnector/acon_pots.c \
    file://src/aconnector/acon_powers.c \
    file://src/aconnector/acon_spectra.c \
    file://src/aconnector/acon_targets.c \
    file://src/aconnector/acon_tracks.c \
    file://src/ainjector/ainj_targets.c \
    file://src/amessage/amsg_categories.c \
    file://src/amessage/amsg_hops.c \
    file://src/amessage/amsg_pots.c \
    file://src/amessage/amsg_powers.c \
    file://src/amessage/amsg_spectra.c \
    file://src/amessage/amsg_targets.c \
    file://src/amessage/amsg_tracks.c \
    file://src/amodule/amod_classify.c \
    file://src/amodule/amod_istft.c \
    file://src/amodule/amod_mapping.c \
    file://src/amodule/amod_noise.c \
    file://src/amodule/amod_resample.c \
    file://src/amodule/amod_ssl.c \
    file://src/amodule/amod_sss.c \
    file://src/amodule/amod_sst.c \
    file://src/amodule/amod_stft.c \
    file://src/amodule/amod_volume.c \
    file://src/asink/asnk_categories.c \
    file://src/asink/asnk_hops.c \
    file://src/asink/asnk_pots.c \
    file://src/asink/asnk_powers.c \
    file://src/asink/asnk_spectra.c \
    file://src/asink/asnk_tracks.c \
    file://src/asource/asrc_hops.c \
    file://src/connector/con_categories.c \
    file://src/connector/con_hops.c \
    file://src/connector/con_pots.c \
    file://src/connector/con_powers.c \
    file://src/connector/con_spectra.c \
    file://src/connector/con_targets.c \
    file://src/connector/con_tracks.c \
    file://src/injector/inj_targets.c \
    file://src/message/msg_categories.c \
    file://src/message/msg_hops.c \
    file://src/message/msg_pots.c \
    file://src/message/msg_powers.c \
    file://src/message/msg_spectra.c \
    file://src/message/msg_targets.c \
    file://src/message/msg_tracks.c \
    file://src/module/mod_classify.c \
    file://src/module/mod_istft.c \
    file://src/module/mod_mapping.c \
    file://src/module/mod_noise.c \
    file://src/module/mod_resample.c \
    file://src/module/mod_ssl.c \
    file://src/module/mod_sss.c \
    file://src/module/mod_sst.c \
    file://src/module/mod_stft.c \
    file://src/module/mod_volume.c \
    file://src/sink/snk_categories.c \
    file://src/sink/snk_hops.c \
    file://src/sink/snk_pots.c \
    file://src/sink/snk_powers.c \
    file://src/sink/snk_spectra.c \
    file://src/sink/snk_tracks.c \
    file://src/source/src_hops.c \
    file://demo/odaslive/main.c \
    file://demo/odaslive/main.c \
    file://demo/odaslive/configs.c \
    file://demo/odaslive/configs.h \
    file://demo/odaslive/objects.c \
    file://demo/odaslive/objects.h \
    file://demo/odaslive/parameters.c \
    file://demo/odaslive/parameters.h \
    file://demo/odaslive/profiler.c \
    file://demo/odaslive/profiler.h \
    file://demo/odaslive/threads.c \
    file://demo/odaslive/threads.h \
    file://Makefile"

S = "${WORKDIR}"

DEPENDS = " fftwf libconfig"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 odaslive ${D}${bindir}
}
