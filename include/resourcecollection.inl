// resourcecollection.hpp

#include <typeinfo>

template<typename Identifier>
const char* getTypeID(Identifier id)
{
    if (typeid(id) == typeid(TextureID::none))
        return texture_names[static_cast<size_t>(id)];
    if (typeid(id) == typeid(FontID::none))
        return font_names[static_cast<size_t>(id)];
    if (typeid(id) == typeid(SoundEffectID::none))
        return soundeffect_names[static_cast<size_t>(id)];
    if (typeid(id) == typeid(ShaderID::none))
        return shader_names[static_cast<size_t>(id)];

    return "unknown type";
}

template<typename Resource, typename Identifier>
void ResourceCollection<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
    Log::Logger logger("ResColle::load"); 
    logger.info() << "Attempting to load " << getTypeID(id) << " from file \"" << filename << "\"...";

    if (auto resource = m_resourceMap.find(id) != m_resourceMap.end())
    {
        logger.warning() << "Resource with ID \"" << getTypeID(id) << "\" already loaded";
        return;
    }

    auto resource = std::make_unique<Resource>();
    if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResColle::load failed! " + filename);
    
    insertResource(id, std::move(resource));
    logger.info() << "\"" << getTypeID(id) << "\" loaded successfully!";
}

template<typename Resource, typename Identifier>
template<typename Parameter>
void ResourceCollection<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
    Log::Logger logger("ResColle::load"); 
    logger.info() << "Attempting to load " << getTypeID(id) << " from file \"" << filename << "\"...";

    if (auto resource = m_resourceMap.find(id) != m_resourceMap.end())
    {
        logger.warning() << "Resource with ID \"" << getTypeID(id) << "\" already loaded";
        return;
    }

    auto resource = std::make_unique<Resource>();
    if (!resource->loadFromFile(filename, secondParam))
        throw std::runtime_error("ResColle::load failed! " + filename);

    insertResource(id, std::move(resource));
    logger.info() << "\"" << getTypeID(id) << "\" loaded successfully!";
}

template<typename Resource, typename Identifier>
Resource& ResourceCollection<Resource, Identifier>::get(Identifier id)
{
    Log::Logger logger("ResColle::get");
    logger.info() << "Retrieving resource with ID \"" << getTypeID(id) << "\"";
    auto found = m_resourceMap.find(id);
    assert(found != m_resourceMap.end());

    return *found->second;
}

template<typename Resource, typename Identifier>
const Resource& ResourceCollection<Resource, Identifier>::get(Identifier id) const
{
    Log::Logger logger("ResColle::get const");
    logger.info() << "Retrieving resource with ID \"" << getTypeID(id) << "\"";

    auto found = m_resourceMap.find(id);
    assert(found != m_resourceMap.end());

    return *found->second;
}

template<typename Resource, typename Identifier>
void ResourceCollection<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
    auto inserted = m_resourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}
