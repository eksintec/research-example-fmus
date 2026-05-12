#ifndef HELPER_H
#define HELPER_H

#define UNUSED(x) (void) x;

#define EMPTY_GETTER(TYPE) \
fmi2Status fmi2Get ## TYPE(fmi2Component instance, \
    const fmi2ValueReference valueReferences[], \
    size_t nValueReferences, \
    fmi2 ## TYPE  values[]) { \
    UNUSED(instance) UNUSED(valueReferences) UNUSED(nValueReferences) UNUSED(values) \
    return fmi2Error; \
}

#define EMPTY_SETTER(TYPE) \
fmi2Status fmi2Set ## TYPE (fmi2Component instance, \
    const fmi2ValueReference valueReferences[], \
    size_t nValueReferences, \
    const fmi2 ## TYPE values[]) { \
    UNUSED(instance) UNUSED(valueReferences) UNUSED(nValueReferences) UNUSED(values) \
    return fmi2Error; \
}

#define EMPTY_GETTER_SETTER(TYPE) \
EMPTY_GETTER(TYPE) \
EMPTY_SETTER(TYPE)

#endif
