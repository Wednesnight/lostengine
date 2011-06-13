#ifndef LOST_RESOURCE_FORWARD_H
#define LOST_RESOURCE_FORWARD_H

namespace lost
{
namespace resource
{

struct Loader;
typedef lost::shared_ptr<Loader> LoaderPtr;
  
struct Repository;
typedef lost::shared_ptr<Repository> RepositoryPtr;

struct ApplicationResourceRepository;
typedef lost::shared_ptr<ApplicationResourceRepository> ApplicationResourceRepositoryPtr;

struct FilesystemRepository;
typedef lost::shared_ptr<FilesystemRepository> FilesystemRepositoryPtr;

struct DefaultLoader;
typedef lost::shared_ptr<DefaultLoader> DefaultLoaderPtr;  

}
}

#endif
