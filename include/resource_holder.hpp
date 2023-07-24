#ifndef YA2DE_RESOURCE_HOLDER_HPP
#define YA2DE_RESOURCE_HOLDER_HPP

#include <format>
#include <map>
#include <memory>
#include <stdexcept>
#include <string_view>

#include <cassert>

template <typename Resource>
class ResourceHolder
{
public:
    template <typename AdditionalParameter>
    void            loadFromFile(unsigned int id, const std::string& filename, AdditionalParameter additionalParameter);
    void            loadFromFile(unsigned int id, const std::string& filename);
    Resource&       get(unsigned int id);
    const Resource& get(unsigned int id) const;

private:
    void insertResource(unsigned int id, std::unique_ptr<Resource> resource);
    void removeResource(unsigned int id);

    std::map<unsigned int, std::unique_ptr<Resource>> m_resourceMap;
};

template <typename Resource>
void ResourceHolder<Resource>::removeResource(unsigned int id)
{
    auto found = m_resourceMap.find(id);
    assert(found != m_resourceMap.end());
    m_resourceMap.erase(found);
}

template <typename Resource>
void ResourceHolder<Resource>::insertResource(unsigned int id, std::unique_ptr<Resource> resource)
{
    auto inserted = m_resourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

template <typename Resource>
const Resource& ResourceHolder<Resource>::get(unsigned int id) const
{
    auto found = m_resourceMap.find(id);
    assert(found != m_resourceMap.end());

    return *found->second;
}

template <typename Resource>
Resource& ResourceHolder<Resource>::get(unsigned int id)
{
    auto found = m_resourceMap.find(id);
    assert(found != m_resourceMap.end());

    return *found->second;
}

template <typename Resource>
void ResourceHolder<Resource>::loadFromFile(unsigned int id, const std::string& filename)
{
    auto resource = std::make_unique<Resource>();
    if (!resource->loadFromFile(filename))
    {
        throw std::runtime_error(std::format("ResourceHolder::load - Failed to load {}", filename));
    }

    insertResource(id, std::move(resource));
}

template <typename Resource>
template <typename AdditionalParameter>
void ResourceHolder<Resource>::loadFromFile(const unsigned int id, const std::string& filename, AdditionalParameter additionalParameter)
{
    auto resource = std::make_unique<Resource>();
    if (!resource->loadFromFile(filename, additionalParameter))
    {
        throw std::runtime_error(std::format("ResourceHolder::load - Failed to load {}", filename));
    }

    insertResource(id, std::move(resource));
}

#endif //YA2DE_RESOURCE_HOLDER_HPP
