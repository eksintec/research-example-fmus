#include <fmi2Functions.h>
#include <helper.h>

fmi2Status fmi2GetFMUstate(fmi2Component c, fmi2FMUstate* FMUstate) {

    UNUSED(c)
    UNUSED(FMUstate)

    return fmi2Error;
}

fmi2Status fmi2SetFMUstate(fmi2Component c, fmi2FMUstate FMUstate) {

    UNUSED(c)
    UNUSED(FMUstate)
    
    return fmi2Error;
}

fmi2Status fmi2FreeFMUstate(fmi2Component c, fmi2FMUstate* FMUstate) {

    UNUSED(c)
    UNUSED(FMUstate)
    
    return fmi2Error;
}

fmi2Status fmi2SerializedFMUstateSize(fmi2Component c,
        fmi2FMUstate  FMUstate,
    size_t* size) {
    UNUSED(c)
    UNUSED(FMUstate)
    UNUSED(size)
    
    return fmi2Error;
}

fmi2Status fmi2SerializeFMUstate(fmi2Component c,
        fmi2FMUstate  FMUstate,
    fmi2Byte serializedState[],
    size_t size) {

    UNUSED(c)
    UNUSED(FMUstate)
    UNUSED(serializedState)
    UNUSED(size)
    
    return fmi2Error;
}

fmi2Status fmi2DeSerializeFMUstate(fmi2Component c,
    const fmi2Byte serializedState[],
    size_t size,
    fmi2FMUstate* FMUstate) {

    UNUSED(c)
    UNUSED(serializedState)
    UNUSED(size)
    UNUSED(FMUstate)
    
       return fmi2Error;
}
