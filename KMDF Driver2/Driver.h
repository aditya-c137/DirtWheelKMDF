/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>

#include "device.h"
#include "queue.h"
#include "trace.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD KMDFDriver2EvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP KMDFDriver2EvtDriverContextCleanup;
EVT_WDF_DRIVER_UNLOAD KMDFDriver2EvtDriverUnload;
//VOID KMDFDriver2EvtDriverUnload2(PDRIVER_OBJECT);

EXTERN_C_END
