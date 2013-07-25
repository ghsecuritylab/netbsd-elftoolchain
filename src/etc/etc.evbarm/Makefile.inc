#	Makefile.inc,v 1.52 2013/06/16 13:47:42 jakllsch Exp
#
#	etc.evbarm/Makefile.inc -- evbarm-specific etc Makefile targets
#

MKIMAGE= ${NETBSDSRCDIR}/distrib/utils/embedded/mkimage

# If you change the list of distributed kernels, don't forget
# to update the release documentation in distrib/notes/common/contents
EVBARM_BOARDS=

.if ${MACHINE_ARCH} == "arm"
# Little endian platforms (armv4 or pre-armv5t)
EVBARM_BOARDS+=		ADI_BRH CP3100 GEMINI GUMSTIX INTEGRATOR \
			IQ80310 IQ80321 MINI2440 MV2120 SMDK2410 \
			SMDK2800 TEAMASA_NPWR TS7200 TWINTAIL KUROBOX_PRO
.endif


IMAGE.rel=	${RELEASEDIR}/${RELEASEMACHINEDIR}
IMAGE.dir=	${IMAGE.rel}/binary/gzimg
IMAGE.kern=	${IMAGE.rel}/binary/kernel
IMAGE.instk=	${IMAGE.rel}/installation/instkernel

__mkimage: .USE
	TOOL_MAKEFS=${TOOL_MAKEFS} \
	TOOL_DISKLABEL=${TOOL_DISKLABEL} \
	TOOL_FDISK=${TOOL_FDISK} \
	TOOL_GZIP=${TOOL_GZIP} \
	TOOL_MKNOD=${TOOL_MKNOD} \
	TOOL_PAX=${TOOL_PAX} \
	TOOL_MTREE=${TOOL_MTREE} \
	HOST_SH=${HOST_SH} \
	${HOST_SH} ${MKIMAGE} -x -h ${.TARGET:S/smp_//} -D ${DESTDIR} \
	    -K ${.ALLSRC} -S ${NETBSDSRCDIR} ${MKI_OPTS.${.TARGET}} \
	    ${IMAGE.dir}/${.TARGET:S/smp_//}.img.gz

.if ${MACHINE_ARCH} == "arm" || ${MACHINE_ARCH} == "earm" \
    || ${MACHINE_ARCH} == "earmhf"
# Little endian platforms with VFP
EVBARM_BOARDS+=		BCM5301X BEAGLEBOARD BEAGLEBOARDXM BEAGLEBONE TISDP2420
smp_beagleboard: ${IMAGE.kern}/netbsd-BEAGLEBOARD.ub.gz __mkimage
.if empty(ALL_KERNELS) || !empty(ALL_KERNELS:MBEAGLEBOARD)
SNAP_MD_POST_DEPS+=	smp_beagleboard
.endif
.endif
.if ${MACHINE_ARCH} == "arm" || ${MACHINE_ARCH} == "earm"
# Little endian platforms (armv5t+)
EVBARM_BOARDS+=		OPENRD SHEEVAPLUG 
.endif

.if ${MACHINE_ARCH} == "armeb"
# Big endian platforms
EVBARM_BOARDS+=		ADI_BRH
.endif

.for board in ${EVBARM_BOARDS}
KERNEL_SETS+=		${board}
BUILD_KERNELS+=		${board}_INSTALL
.endfor

.if ${MACHINE_ARCH} == "arm"
# Little endian platforms.
KERNEL_SETS+=		HDL_G INTEGRATOR_CP IXM1200
.endif

.if ${MACHINE_ARCH} == "arm" || ${MACHINE_ARCH} == "earm" \
    || ${MACHINE_ARCH} == "earmhf"
KERNEL_SETS+=		RPI
BUILD_KERNELS+=		RPI_INSTALL
smp_rpi: ${IMAGE.kern}/netbsd-RPI.bin.gz __mkimage
.if empty(ALL_KERNELS) || !empty(ALL_KERNELS:MRPI)
SNAP_MD_POST_DEPS+=	smp_rpi
.endif
MKI_OPTS.smp_rpi_inst="-b"
smp_rpi_inst: ${IMAGE.instk}/netbsd-RPI_INSTALL.bin.gz __mkimage
.if empty(ALL_KERNELS) || !empty(ALL_KERNELS:MRPI_INSTALL)
SNAP_MD_POST_DEPS+=	smp_rpi_inst
.endif
.endif

.if ${MACHINE_ARCH} == "armeb"
# Big endian platforms.
KERNEL_SETS+=		IXM1200 NSLU2
.endif

snap_md_post: check_DESTDIR check_RELEASEDIR snap_post .WAIT ${SNAP_MD_POST_DEPS}

KERNEL_SUFFIXES=	bin srec ub

INSTALLATION_DIRS+=	binary/gzimg		\
			installation/instkernel