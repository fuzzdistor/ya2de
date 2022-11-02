#ifndef YA2DE_RESOURCECOLLECTION_HPP
#define YA2DE_RESOURCECOLLECTION_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

#include <LoggerCpp/LoggerCpp.h>


template <typename Resource, typename Identifier>
class GenericResourceCollection
{
public:
    void load(Identifier id, const std::string& filename);

    template<typename Parameter>
    void load(Identifier id, const std::string& filename, const Parameter& secondParam);

    Resource& get(Identifier id);
    const Resource& get(Identifier id) const;

private:
    void insertResource(Identifier id, std::unique_ptr<Resource> resource);

    std::map<Identifier, std::unique_ptr<Resource>> m_resourceMap;
};

#include "resourcecollection.inl"
#endif // YA2DE_RESOURCECOLLECTION_HPP

