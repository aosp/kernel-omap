# no Makefile in dbgdrv
#obj-$(CONFIG_PVR_PDUMP) += dbgdrv/

INCDIR=drivers/gpu/pvr

include_dirs := \
	-I$(INCDIR)/eurasia_km/include4 \
        -I$(INCDIR)/eurasia_km/services4/include \
        -I$(INCDIR)/eurasia_km/services4/include/env/linux \
        -I$(INCDIR)/eurasia_km/services4/srvkm/env/linux \
        -I$(INCDIR)/eurasia_km/services4/srvkm/include \
        -I$(INCDIR)/eurasia_km/services4/srvkm/bridged \
	-I$(INCDIR)/eurasia_km/services4/system/include \
	-I$(INCDIR)/eurasia_km/services4/system/omap \
        -I$(INCDIR)/eurasia_km/services4/srvkm/hwdefs \
        -I$(INCDIR)/eurasia_km/services4/srvkm/bridged/sgx \
        -I$(INCDIR)/eurasia_km/services4/srvkm/devices/sgx \
        -Idrivers/video/omap2 \
        -Iinclude/linux

ccflags-y += $(include_dirs)

ccflags-y += \
	-Wall \
	-Werror \
	-D__linux__ \
	-DSGX_FEATURE_MP_CORE_COUNT=1 \
	-DLINUX  \
	-DPVR_BUILD_DIR="\"omap_linux\"" \
	-DPVR_BUILD_TYPE="\"release\"" \
	-DPVRSRV_MODNAME="\"pvrsrvkm\"" \
	-DSGXCORE=544 \
	-DSGX544  \
	-DSUPPORT_SGX544  \
	-DSUPPORT_SGX  \
	-DSGX_CORE_REV=116 \
	-DTRANSFER_QUEUE  \
	-DPVR_SECURE_HANDLES  \
	-DSUPPORT_LISR_MISR_SYNC  \
	-DDISPLAY_CONTROLLER=omaplfb \
	-DPVR_LINUX_MEM_AREA_POOL_MAX_PAGES=0 \
	-DRELEASE  \
	-DSUPPORT_PERCONTEXT_PB  \
	-DSUPPORT_HW_RECOVERY  \
	-DSUPPORT_ACTIVE_POWER_MANAGEMENT  \
	-DSUPPORT_SGX_HWPERF  \
	-DSUPPORT_SGX_LOW_LATENCY_SCHEDULING  \
	-DSUPPORT_SGX_NEW_STATUS_VALS  \
	-DSUPPORT_DBGDRV_EVENT_OBJECTS  \
	-DSGX_FEATURE_SYSTEM_CACHE  \
	-DSGX_FEATURE_MP  \
	-DSYS_USING_INTERRUPTS  \
	-DPVRSRV_NEW_PVR_DPF  \
	-DPVRSRV_NEED_PVR_DPF  \
	-DPVRSRV_NEED_PVR_ASSERT  \
	-DPVRSRV_NEED_PVR_TRACE  \
	-DSUPPORT_PVRSRV_DEVICE_CLASS  \
	-DSUPPORT_PVRSRV_GET_DC_SYSTEM_BUFFER  \
	-DIMG_ADDRSPACE_PHYSADDR_BITS=32 \
	-DSUPPORT_LINUX_X86_WRITECOMBINE  \
	-DSUPPORT_LINUX_X86_PAT  \
	-DSGX_DYNAMIC_TIMING_INFO  \
	-DSYS_CUSTOM_POWERLOCK_WRAP  \
	-DPVR_LINUX_USING_WORKQUEUES  \
	-DPVR_LINUX_MISR_USING_PRIVATE_WORKQUEUE  \
	-DPVR_LINUX_TIMERS_USING_WORKQUEUES  \
	-DLDM_PLATFORM  \
	-DPVR_LDM_DRIVER_REGISTRATION_NAME="\"pvrsrvkm\"" \
	-DSUPPORT_OLD_ION_API  \
	-DXPROC_WORKAROUND_NUM_SHAREABLES=4095 \
	-DPVR_OMAPLFB_DONT_USE_FB_PAN_DISPLAY  \
	-DVS_PRODUCT_VERSION=5 \
	-DSYS_OMAP5_UEVM

#ccflags-$(CONFIG_PVR_BUILD_RELEASE) +=
#        -DPVR_BUILD_TYPE="\"release\"" -DRELEASE
#ccflags-y += \
        -DPVR_BUILD_TYPE="\"release\"" -DRELEASE
#        -DPVR_BUILD_TYPE="\"debug\"" -DDEBUG -DDEBUG_LINUX_MEMORY_ALLOCATIONS -DDEBUG_LINUX_MMAP_AREAS -DDEBUG_LINUX_MEM_AREAS -DDEBUG_BRIDGE_KM

#ccflags-$(CONFIG_PVR_BUILD_DEBUG) += \
        -DPVR_BUILD_TYPE="\"debug\"" -DDEBUG -DDEBUG_LINUX_MEMORY_ALLOCATIONS -DDEBUG_LINUX_MMAP_AREAS -DDEBUG_LINUX_MEM_AREAS -DDEBUG_BRIDGE_KM

ENVDIR = eurasia_km/services4/srvkm/env/linux
COMMONDIR = eurasia_km/services4/srvkm/common
BRIDGEDDIR = eurasia_km/services4/srvkm/bridged
SGXDIR = eurasia_km/services4/srvkm/devices/sgx
SYSCONFIGDIR = eurasia_km/services4/system/omap
DCDIR = eurasia_km/services4/3rdparty/dc_omapfb3_linux
BCDIR = eurasia_km/services4/3rdparty/bufferclass_example

pvrsrvkm-y += $(ENVDIR)/osfunc.o \
	  $(ENVDIR)/mutils.o \
	  $(ENVDIR)/mmap.o \
	  $(ENVDIR)/module.o \
	  $(ENVDIR)/pdump.o \
	  $(ENVDIR)/proc.o \
	  $(ENVDIR)/pvr_bridge_k.o \
	  $(ENVDIR)/pvr_debug.o \
	  $(ENVDIR)/mm.o \
	  $(ENVDIR)/mutex.o \
	  $(ENVDIR)/event.o \
	  $(ENVDIR)/osperproc.o

pvrsrvkm-y += $(COMMONDIR)/buffer_manager.o \
	  $(COMMONDIR)/devicemem.o \
	  $(COMMONDIR)/deviceclass.o \
	  $(COMMONDIR)/handle.o \
	  $(COMMONDIR)/hash.o \
	  $(COMMONDIR)/lists.o \
	  $(COMMONDIR)/mem.o \
	  $(COMMONDIR)/mem_debug.o \
	  $(COMMONDIR)/metrics.o \
	  $(COMMONDIR)/osfunc_common.o \
	  $(COMMONDIR)/pdump_common.o \
	  $(COMMONDIR)/perproc.o \
	  $(COMMONDIR)/power.o \
	  $(COMMONDIR)/pvrsrv.o \
	  $(COMMONDIR)/queue.o \
	  $(COMMONDIR)/ra.o \
	  $(COMMONDIR)/refcount.o \
	  $(COMMONDIR)/resman.o

pvrsrvkm-y += $(BRIDGEDDIR)/bridged_support.o \
	  $(BRIDGEDDIR)/bridged_pvr_bridge.o \
	  $(BRIDGEDDIR)/sgx/bridged_sgx_bridge.o

pvrsrvkm-y += $(SYSCONFIGDIR)/sysconfig.o \
	  $(SYSCONFIGDIR)/sysutils.o

pvrsrvkm-y += $(SGXDIR)/sgxinit.o \
	  $(SGXDIR)/sgxpower.o \
	  $(SGXDIR)/sgxreset.o \
	  $(SGXDIR)/sgxutils.o \
	  $(SGXDIR)/sgxkick.o \
	  $(SGXDIR)/sgxtransfer.o \
	  $(SGXDIR)/mmu.o \
	  $(SGXDIR)/pb.o

omaplfb-y += $(DCDIR)/omaplfb_displayclass.o \
	  $(DCDIR)/omaplfb_linux.o

#bufferclass-y += $(BCDIR)/bufferclass_example.o \
	  $(BCDIR)/bufferclass_example_linux.o \
	  $(BCDIR)/bufferclass_example_private.o

obj-y += pvrsrvkm.o
obj-y += omaplfb.o
#obj-y += bufferclass.o
