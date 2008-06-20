#ifndef LOST_COMMON_SERIALIZATIONHELPER_H
#define LOST_COMMON_SERIALIZATIONHELPER_H


#define BOOST_SERIALIZATION_NON_INTRUSIVE_BASE_OBJECT_NVP(basename,basetype,derived)    \
    boost::serialization::make_nvp(                                                     \
        #basename,                                                                     \
        boost::serialization::base_object<basetype>(derived)                            \
    )


#endif
