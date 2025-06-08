#!/bin/bash
# Script to build a minimal Linux system for QEMU
# Author: Siddhant Jajoo (updated)

set -e
set -u

OUTDIR=/tmp/aeld
KERNEL_REPO=git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
KERNEL_VERSION=v5.15.163
BUSYBOX_VERSION=1_33_1
FINDER_APP_DIR=$(realpath $(dirname $0))
ARCH=arm64
CROSS_COMPILE=aarch64-none-linux-gnu-

if [ $# -lt 1 ]
then
	echo "Using default directory ${OUTDIR} for output"
else
	OUTDIR=$1
	echo "Using passed directory ${OUTDIR} for output"
fi

mkdir -p ${OUTDIR} || { echo "Failed to create output directory"; exit 1; }

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/linux-stable" ]; then
    echo "CLONING GIT LINUX STABLE VERSION ${KERNEL_VERSION} IN ${OUTDIR}"
<<<<<<< HEAD
if [ -d "${OUTDIR}/rootfs" ]
then
	echo "Deleting rootfs directory at ${OUTDIR}/rootfs and starting over"
	sudo rm -rf ${OUTDIR}/rootfs
fi

=======
    git clone ${KERNEL_REPO} --depth 1 --single-branch --branch ${KERNEL_VERSION}
fi

if [ -e ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ]; then
    echo "Skipping kernel build — Image already exists"
else
    cd linux-stable
    echo "Building kernel version ${KERNEL_VERSION}"
    git checkout ${KERNEL_VERSION}
    make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} mrproper
    make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} defconfig
    make -j$(nproc) ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} all
fi

cp ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ${OUTDIR}/

echo "Creating the staging directory for the root filesystem"
cd "$OUTDIR"
if [ -d "${OUTDIR}/rootfs" ]
then
	echo "Deleting rootfs directory at ${OUTDIR}/rootfs and starting over"
	sudo rm -rf ${OUTDIR}/rootfs
fi

>>>>>>> 83f2a1cb682f141dbcc7cb29527c65b4857abd57
mkdir -p rootfs/{bin,sbin,etc,proc,sys,usr/{bin,sbin},dev,home,lib,lib64,tmp,var,lib/modules}
chmod 755 rootfs

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/busybox" ]
then
    git clone git://busybox.net/busybox.git
    cd busybox
    git checkout ${BUSYBOX_VERSION}
    make distclean
    make defconfig
else
    cd busybox
fi

make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE}
make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} CONFIG_PREFIX=${OUTDIR}/rootfs install
# Create a valid /init script for the kernel to start
cat > "${OUTDIR}/rootfs/init" <<'EOF'
#!/bin/sh
mount -t proc none /proc
mount -t sysfs none /sys
echo "Init script running. Starting shell..."
exec /bin/sh
EOF

chmod +x ${OUTDIR}/rootfs/init
echo "Library dependencies"

SYSROOT=$(${CROSS_COMPILE}gcc -print-sysroot)
cp -a ${SYSROOT}/lib/ld-linux-aarch64.so.1 ${OUTDIR}/rootfs/lib/
cp -a ${SYSROOT}/lib64/ld-*.so* ${OUTDIR}/rootfs/lib64/ || true
cp -a ${SYSROOT}/lib64/libc.so* ${OUTDIR}/rootfs/lib64/ || true
cp -a ${SYSROOT}/lib64/libm.so* ${OUTDIR}/rootfs/lib64/ || true
cp -a ${SYSROOT}/lib64/libresolv.so* ${OUTDIR}/rootfs/lib64/ || true

sudo mknod -m 666 ${OUTDIR}/rootfs/dev/null c 1 3
sudo mknod -m 600 ${OUTDIR}/rootfs/dev/console c 5 1

cd ${FINDER_APP_DIR}
make clean
make CROSS_COMPILE=${CROSS_COMPILE}

mkdir -p ${OUTDIR}/rootfs/home
cp writer ${OUTDIR}/rootfs/home/
cp finder.sh finder-test.sh ${OUTDIR}/rootfs/home/
cp conf/username.txt conf/assignment.txt ${OUTDIR}/rootfs/home/
sed -i 's|conf/username.txt|username.txt|g' ${OUTDIR}/rootfs/home/finder-test.sh

sed -i 's|conf/assignment.txt|assignment.txt|g' ${OUTDIR}/rootfs/home/finder-test.sh
chmod +x ${OUTDIR}/rootfs/home/finder.sh
chmod +x ${OUTDIR}/rootfs/home/finder-test.sh

cp autorun-qemu.sh ${OUTDIR}/rootfs/home/
chmod +x ${OUTDIR}/rootfs/home/autorun-qemu.sh


cd ${OUTDIR}/rootfs
sudo chown -R root:root *
find . | cpio -H newc -ov --owner root:root > ${OUTDIR}/initramfs.cpio
cd ${OUTDIR}
gzip -f initramfs.cpio
chmod +x ${OUTDIR}/rootfs/home/*.sh

echo "Build complete: Image and initramfs.cpio.gz are ready in ${OUTDIR}"
exit 0

