/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_KMDFDriver2,
    0xfdda49af,0x8273,0x4261,0xa5,0xde,0xd0,0xba,0x92,0x57,0xd6,0xde);
// {fdda49af-8273-4261-a5de-d0ba9257d6de}
