#include "ZooKeeperImpl.h"

#include <functional>
#include <string.h>

#include <zookeeper/zookeeper.h>

#include "ZooKeeperException.h"
#include "ZooKeeperNodeManager.h"
#include "ZooKeeperSessionManager.h"

#include "Spot/Common/Utility/System.h"
#include "Spot/Common/ZooKeeper/IZooKeeperNodeEventHandler.h"
#include "Spot/Common/ZooKeeper/IZooKeeperSessionEventHandler.h"


namespace Spot { namespace Common { namespace ZooKeeper
{
  ZooKeeperImpl::ZooKeeperImpl( const std::string& host, int connectionTimeout, int sessionExpirationTimeout, int deterministicConnectionOrder ) :
    m_logger( nullptr ),
    m_zooKeeperNodeManager( nullptr ),
    m_zooKeeperSessionManager( nullptr )
  {
    zoo_set_debug_level( ZOO_LOG_LEVEL_DEBUG );

    m_zooKeeperSessionManager = new ZooKeeperSessionManager( host, connectionTimeout, sessionExpirationTimeout, deterministicConnectionOrder );
    m_zooKeeperNodeManager = new ZooKeeperNodeManager( m_zooKeeperSessionManager );
  }


  ZooKeeperImpl::~ZooKeeperImpl() noexcept
  {
    if( m_zooKeeperNodeManager != nullptr )
    {
      delete m_zooKeeperNodeManager;
    }

    if( m_zooKeeperSessionManager != nullptr )
    {
      delete m_zooKeeperSessionManager;
    }
  }


  void ZooKeeperImpl::RegisterLogger( Logger::ILogger* logger )
  {
    if( m_logger != nullptr )
    {
      char result[1024];
      sprintf( result, "[%s:%i] Logging event handler already initialized", __FUNCTION__, __LINE__ );

      throw std::runtime_error( result );
    }

    m_logger = logger;

    LOGGER_DEBUG( m_logger, "Logger registered" );
  }


  void ZooKeeperImpl::UnregisterLogger()
  {
    if( m_logger == nullptr )
    {
      char result[1024];
      sprintf( result, "[%s:%i] Logging event handler not initialized", __FUNCTION__, __LINE__ );

      throw std::runtime_error( result );
    }

    LOGGER_DEBUG( m_logger, "Logger unregistered" );

    m_logger = nullptr;
  }


  void ZooKeeperImpl::RegisterSessionEventHandler( IZooKeeperSessionEventHandler* eventHandler )
  {
    m_zooKeeperSessionManager->RegisterEventHandler( eventHandler );
  }


  void ZooKeeperImpl::UnregisterSessionEventHandler()
  {
    m_zooKeeperSessionManager->UnregisterEventHandler();

    LOGGER_DEBUG( m_logger, "Session event handler unregistered" );
 }


  void ZooKeeperImpl::Initialize()
  {
    m_zooKeeperSessionManager->Initialize();

    LOGGER_DEBUG( m_logger, "Initialized" );
  }


  void ZooKeeperImpl::Uninitialize()
  {
    m_zooKeeperSessionManager->Uninitialize();

    LOGGER_DEBUG( m_logger, "Uninitialized" );
  }


  const std::string& ZooKeeperImpl::GetHost() const noexcept
  {
    return m_zooKeeperSessionManager->GetHost();
  }


  int ZooKeeperImpl::GetConnectionTimeout() const noexcept
  {
    return m_zooKeeperSessionManager->GetConnectionTimeout();
  }


  int ZooKeeperImpl::GetExpirationTimeout() const noexcept
  {
    return m_zooKeeperSessionManager->GetExpirationTimeout();
  }


  int ZooKeeperImpl::GetDeterministicConnectionOrder() const noexcept
  {
    return m_zooKeeperSessionManager->GetDeterministicConnectionOrder();
  }


  ZooKeeperStringResult ZooKeeperImpl::GetNode( const std::string& path )
  {
    return m_zooKeeperNodeManager->GetNode( path );
  }


  ZooKeeperStringResult ZooKeeperImpl::GetNode( const std::string& path, IZooKeeperNodeEventHandler* eventHandler )
  {
    return m_zooKeeperNodeManager->GetNode( path, eventHandler );
  }


  ZooKeeperStringVectorResult ZooKeeperImpl::GetNodeChildren( const std::string& path )
  {
    return m_zooKeeperNodeManager->GetNodeChildren( path );
  }


  ZooKeeperStringVectorResult ZooKeeperImpl::GetNodeChildren( const std::string& path, IZooKeeperNodeEventHandler* eventHandler )
  {
    return m_zooKeeperNodeManager->GetNodeChildren( path, eventHandler );
  }


  bool ZooKeeperImpl::CreateNode( const std::string& path, const std::string& data, ZooKeeperNodeType nodeType )
  {
    return m_zooKeeperNodeManager->CreateNode( path, data, nodeType );
  }


  std::string ZooKeeperImpl::CreateSequentialNode( const std::string& path, const std::string& data, ZooKeeperNodeType nodeType )
  {
    return m_zooKeeperNodeManager->CreateSequentialNode( path, data, nodeType );
  }


  void ZooKeeperImpl::ChangeNode( const std::string& path, const std::string& data )
  {
    m_zooKeeperNodeManager->ChangeNode( path, data );

    char result[1024];
    sprintf( result, "Node <%s> data changed to <%s>", path.c_str(), data.c_str() );

    LOGGER_DEBUG( m_logger, result );
  }


  void ZooKeeperImpl::DeleteNode( const std::string& path )
  {
    m_zooKeeperNodeManager->DeleteNode( path );

    char result[1024];
    sprintf( result, "Node <%s> deleted", path.c_str() );

    LOGGER_DEBUG( m_logger, result );
  }


  void ZooKeeperImpl::AddNodeEventHandler( const std::string& path, IZooKeeperNodeEventHandler* eventHandler )
  {
    m_zooKeeperNodeManager->AddEventHandler( path, eventHandler );

    char result[1024];
    sprintf( result, "Node event handler added to path <%s>", path.c_str() );

    LOGGER_DEBUG( m_logger, result );
  }


  void ZooKeeperImpl::RemoveNodeEventHandler( const std::string& path )
  {
    m_zooKeeperNodeManager->RemoveEventHandler( path );

    char result[1024];
    sprintf( result, "Node event handler removed from path <%s>", path.c_str() );

    LOGGER_DEBUG( m_logger, result );
  }

} } } // namespaces
