{
  'variables': {
    'target_arch%': 'x64'
  },
  'target_defaults': {
    'default_configuration': 'Release',
    'configurations': {
      'Debug': {
        'defines': [ 'DEBUG', '_DEBUG' ],
      },
      'Release': {
        'defines': [ 'NDEBUG', 'PIC' ],
      }
    },
    'include_dirs': [
      '/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/include/libxml2',
      'deps/wslay/lib/includes',
      'src/includes',
      'config/<(OS)/<(target_arch)'
    ],
    'defines': [
      'PIC',
      'HAVE_CONFIG_H'
    ],
    'conditions': [
      [ 'OS=="mac"', {
        'xcode_settings': {
          'OTHER_CPLUSPLUSFLAGS': ['-std=c++11', '-stdlib=libc++'],
        },
        'libraries': ['-lxml2', '-lz'],
        'ldflags': ['-framework CoreFoundation', '-framework Security'],
      }],
    ],
  },
  'targets': [
    {
      'target_name': 'wslay',
      'type': 'static_library',
      'sources': [
        'deps/wslay/lib/wslay_event.c',
        'deps/wslay/lib/wslay_frame.c',
        'deps/wslay/lib/wslay_net.c',
        'deps/wslay/lib/wslay_queue.c',
      ],
    },
    {
      'target_name': 'aria2',
      'type': 'static_library',
      'conditions': [
        [ 'OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'GCC_ENABLE_CPP_RTTI': 'YES',
            'MACOSX_DEPLOYMENT_TARGET': '10.8',
          },
        }],
      ],
      'sources': [
        'src/option_processing.cc',
        'src/version_usage.cc',
        'src/Context.cc',
        'src/SocketCore.cc',
        'src/Command.cc',
        'src/AbstractCommand.cc',
        'src/InitiateConnectionCommandFactory.cc',
        'src/DownloadCommand.cc',
        'src/HttpInitiateConnectionCommand.cc',
        'src/HttpRequestCommand.cc',
        'src/HttpResponseCommand.cc',
        'src/HttpProxyRequestCommand.cc',
        'src/HttpProxyResponseCommand.cc',
        'src/HttpDownloadCommand.cc',
        'src/HttpHeader.cc',
        'src/HttpConnection.cc',
        'src/FtpConnection.cc',
        'src/FtpInitiateConnectionCommand.cc',
        'src/FtpNegotiationCommand.cc',
        'src/FtpDownloadCommand.cc',
        'src/FtpTunnelRequestCommand.cc',
        'src/FtpTunnelResponseCommand.cc',
        'src/DownloadEngine.cc',
        'src/GrowSegment.cc',
        'src/PiecedSegment.cc',
        'src/SegmentMan.cc',
        'src/util.cc',
        'src/Request.cc',
        'src/Exception.cc',
        'src/FatalException.cc',
        'src/RecoverableException.cc',
        'src/DlAbortEx.cc',
        'src/DlRetryEx.cc',
        'src/DownloadFailureException.cc',
        'src/Logger.cc',
        'src/fmt.cc',
        'src/AbstractDiskWriter.cc',
        'src/DefaultDiskWriter.cc',
        'src/DefaultDiskWriterFactory.cc',
        'src/File.cc',
        'src/Option.cc',
        'src/base32.cc',
        'src/LogFactory.cc',
        'src/TimerA2.cc',
        'src/TimeA2.cc',
        'src/FeatureConfig.cc',
        'src/DownloadEngineFactory.cc',
        'src/SpeedCalc.cc',
        'src/PeerStat.cc',
        'src/NetStat.cc',
        'src/BitfieldMan.cc',
        'src/SimpleRandomizer.cc',
        'src/HttpResponse.cc',
        'src/HttpRequest.cc',
        'src/Range.cc',
        'src/AbstractProxyRequestCommand.cc',
        'src/AbstractProxyResponseCommand.cc',
        'src/Netrc.cc',
        'src/AuthConfig.cc',
        'src/AbstractAuthResolver.cc',
        'src/DefaultAuthResolver.cc',
        'src/NetrcAuthResolver.cc',
        'src/AuthConfigFactory.cc',
        'src/OptionParser.cc',
        'src/OptionHandlerFactory.cc',
        'src/OptionHandler.cc',
        'src/OptionHandlerImpl.cc',
        'src/AbstractOptionHandler.cc',
        'src/NameResolver.cc',
        'src/RequestGroup.cc',
        'src/RequestGroupMan.cc',
        'src/FileAllocationCommand.cc',
        'src/FillRequestGroupCommand.cc',
        'src/FileAllocationDispatcherCommand.cc',
        'src/FileAllocationEntry.cc',
        'src/StreamFileAllocationEntry.cc',
        'src/MultiUrlRequestInfo.cc',
        'src/UriListParser.cc',
        'src/RealtimeCommand.cc',
        'src/RequestGroupEntry.cc',
        'src/Cookie.cc',
        'src/HttpHeaderProcessor.cc',
        'src/FileEntry.cc',
        'src/Platform.cc',
        'src/TimeBasedCommand.cc',
        'src/AutoSaveCommand.cc',
        'src/DefaultPieceStorage.cc',
        'src/UnknownLengthPieceStorage.cc',
        'src/PieceStatMan.cc',
        'src/ConsoleStatCalc.cc',
        'src/TransferStat.cc',
        'src/DefaultBtProgressInfoFile.cc',
        'src/SingleFileAllocationIterator.cc',
        'src/ContentTypeRequestGroupCriteria.cc',
        'src/DownloadHandler.cc',
        'src/DownloadHandlerConstants.cc',
        'src/DownloadHandlerFactory.cc',
        'src/HaveEraseCommand.cc',
        'src/Piece.cc',
        'src/CheckIntegrityEntry.cc',
        'src/PieceHashCheckIntegrityEntry.cc',
        'src/StreamCheckIntegrityEntry.cc',
        'src/DiskAdaptor.cc',
        'src/AbstractSingleDiskAdaptor.cc',
        'src/DirectDiskAdaptor.cc',
        'src/MultiDiskAdaptor.cc',
        'src/MultiFileAllocationIterator.cc',
        'src/ByteArrayDiskWriter.cc',
        'src/DownloadContext.cc',
        'src/TimedHaltCommand.cc',
        'src/CUIDCounter.cc',
        'src/DNSCache.cc',
        'src/DownloadResult.cc',
        'src/help_tags.cc',
        'src/prefs.cc',
        'src/ProtocolDetector.cc',
        'src/HttpSkipResponseCommand.cc',
        'src/InitiateConnectionCommand.cc',
        'src/FtpFinishDownloadCommand.cc',
        'src/A2STR.cc',
        'src/RarestPieceSelector.cc',
        'src/Signature.cc',
        'src/ServerStat.cc',
        'src/ServerStatMan.cc',
        'src/AdaptiveURISelector.cc',
        'src/InorderURISelector.cc',
        'src/FeedbackURISelector.cc',
        'src/NsCookieParser.cc',
        'src/CookieStorage.cc',
        'src/SocketBuffer.cc',
        'src/SocketRecvBuffer.cc',
        'src/OptionHandlerException.cc',
        'src/URIResult.cc',
        'src/SelectEventPoll.cc',
        'src/LongestSequencePieceSelector.cc',
        'src/bitfield.cc',
        'src/CreateRequestCommand.cc',
        'src/wallclock.cc',
        'src/download_helper.cc',
        'src/MetadataInfo.cc',
        'src/SessionSerializer.cc',
        'src/ValueBase.cc',
        'src/ContextAttribute.cc',
        'src/TorrentAttribute.cc',
        'src/AdaptiveFileAllocationIterator.cc',
        'src/TruncFileAllocationIterator.cc',
        'src/StreamFilter.cc',
        'src/SinkStreamFilter.cc',
        'src/ChunkedDecodingStreamFilter.cc',
        'src/NullSinkStreamFilter.cc',
        'src/uri.cc',
        'src/uri_split.c',
        'src/cookie_helper.cc',
        'src/json.cc',
        'src/JsonParser.cc',
        'src/ValueBaseStructParserStateImpl.cc',
        'src/ValueBaseStructParserStateMachine.cc',
        'src/HttpServerBodyCommand.cc',
        'src/RpcRequest.cc',
        'src/RpcMethod.cc',
        'src/RpcMethodImpl.cc',
        'src/RpcMethodFactory.cc',
        'src/RpcResponse.cc',
        'src/HttpListenCommand.cc',
        'src/HttpServerCommand.cc',
        'src/HttpServerResponseCommand.cc',
        'src/AbstractHttpServerResponseCommand.cc',
        'src/HttpServer.cc',
        'src/DefaultStreamPieceSelector.cc',
        'src/InorderStreamPieceSelector.cc',
        'src/GeomStreamPieceSelector.cc',
        'src/MetalinkHttpEntry.cc',
        'src/console.cc',
        'src/IOFile.cc',
        'src/BufferedFile.cc',
        'src/paramed_string.cc',
        'src/rpc_helper.cc',
        'src/WatchProcessCommand.cc',
        'src/UnknownOptionException.cc',
        'src/Notifier.cc',
        'src/XmlRpcRequestParserController.cc',
        'src/WrDiskCache.cc',
        'src/WrDiskCacheEntry.cc',
        'src/GroupId.cc',
        'src/SaveSessionCommand.cc',
        'src/BackupIPv4ConnectCommand.cc',
        'src/ConnectCommand.cc',
        'src/WebSocketSession.cc',
        'src/WebSocketSessionMan.cc',
        'src/WebSocketResponseCommand.cc',
        'src/WebSocketInteractionCommand.cc',
        'src/XmlAttr.cc',
        'src/XmlParser.cc',
        'src/Xml2XmlParser.cc',
        'src/XmlRpcRequestParserStateMachine.cc',
        'src/XmlRpcRequestParserStateImpl.cc',
        'src/XmlRpcDiskWriter.cc',
        'src/AppleMessageDigestImpl.cc',
        'src/AppleTLSContext.cc',
        'src/AppleTLSSession.cc',
        'src/InternalDHKeyExchange.cc',
        'src/InternalARC4Encryptor.cc',
        'src/GZipEncoder.cc',
        'src/GZipDecodingStreamFilter.cc',
        'src/GZipFile.cc',
        'src/IteratableChunkChecksumValidator.cc',
        'src/IteratableChecksumValidator.cc',
        'src/CheckIntegrityDispatcherCommand.cc',
        'src/CheckIntegrityCommand.cc',
        'src/ChecksumCheckIntegrityEntry.cc',
        'src/message_digest_helper.cc',
        'src/Checksum.cc',
        'src/ChunkChecksum.cc',
        'src/MessageDigest.cc',
        'src/PeerAbstractCommand.cc',
        'src/PeerInitiateConnectionCommand.cc',
        'src/PeerInteractionCommand.cc',
        'src/Peer.cc',
        'src/PeerSessionResource.cc',
        'src/PeerConnection.cc',
        'src/PeerListenCommand.cc',
        'src/BtRegistry.cc',
        'src/RequestSlot.cc',
        'src/TrackerWatcherCommand.cc',
        'src/PeerChokeCommand.cc',
        'src/TimeSeedCriteria.cc',
        'src/ShareRatioSeedCriteria.cc',
        'src/UnionSeedCriteria.cc',
        'src/SeedCheckCommand.cc',
        'src/AnnounceTier.cc',
        'src/AnnounceList.cc',
        'src/DefaultPeerStorage.cc',
        'src/BtAnnounce.cc',
        'src/DefaultBtAnnounce.cc',
        'src/BtRuntime.cc',
        'src/AbstractBtMessage.cc',
        'src/SimpleBtMessage.cc',
        'src/BtAllowedFastMessage.cc',
        'src/BtBitfieldMessage.cc',
        'src/BtCancelMessage.cc',
        'src/BtChokeMessage.cc',
        'src/BtHaveAllMessage.cc',
        'src/BtHaveMessage.cc',
        'src/BtHaveNoneMessage.cc',
        'src/BtInterestedMessage.cc',
        'src/BtKeepAliveMessage.cc',
        'src/BtNotInterestedMessage.cc',
        'src/BtPieceMessage.cc',
        'src/BtPortMessage.cc',
        'src/BtRejectMessage.cc',
        'src/BtRequestMessage.cc',
        'src/BtSuggestPieceMessage.cc',
        'src/BtUnchokeMessage.cc',
        'src/BtHandshakeMessage.cc',
        'src/BtBitfieldMessageValidator.cc',
        'src/BtPieceMessageValidator.cc',
        'src/BtHandshakeMessageValidator.cc',
        'src/DefaultBtMessageFactory.cc',
        'src/DefaultBtMessageDispatcher.cc',
        'src/DefaultBtMessageReceiver.cc',
        'src/DefaultBtRequestFactory.cc',
        'src/BtAbortOutstandingRequestEvent.cc',
        'src/DefaultBtInteractive.cc',
        'src/ActivePeerConnectionCommand.cc',
        'src/BtDependency.cc',
        'src/PeerReceiveHandshakeCommand.cc',
        'src/BtSetup.cc',
        'src/BtFileAllocationEntry.cc',
        'src/BtPostDownloadHandler.cc',
        'src/BtCheckIntegrityEntry.cc',
        'src/BtExtendedMessage.cc',
        'src/DefaultExtensionMessageFactory.cc',
        'src/HandshakeExtensionMessage.cc',
        'src/UTPexExtensionMessage.cc',
        'src/UTMetadataExtensionMessage.cc',
        'src/UTMetadataRequestExtensionMessage.cc',
        'src/UTMetadataRejectExtensionMessage.cc',
        'src/UTMetadataDataExtensionMessage.cc',
        'src/UTMetadataRequestTracker.cc',
        'src/UTMetadataRequestFactory.cc',
        'src/UTMetadataPostDownloadHandler.cc',
        'src/magnet.cc',
        'src/DHTNode.cc',
        'src/DHTBucket.cc',
        'src/DHTRoutingTable.cc',
        'src/DHTMessageEntry.cc',
        'src/DHTMessageDispatcherImpl.cc',
        'src/DHTMessageReceiver.cc',
        'src/DHTMessageTracker.cc',
        'src/DHTMessageTrackerEntry.cc',
        'src/DHTMessage.cc',
        'src/DHTConnectionImpl.cc',
        'src/DHTAbstractMessage.cc',
        'src/DHTQueryMessage.cc',
        'src/DHTResponseMessage.cc',
        'src/DHTPingMessage.cc',
        'src/DHTPingReplyMessage.cc',
        'src/DHTFindNodeMessage.cc',
        'src/DHTFindNodeReplyMessage.cc',
        'src/DHTGetPeersMessage.cc',
        'src/DHTGetPeersReplyMessage.cc',
        'src/DHTAnnouncePeerMessage.cc',
        'src/DHTAnnouncePeerReplyMessage.cc',
        'src/DHTUnknownMessage.cc',
        'src/DHTMessageFactoryImpl.cc',
        'src/DHTNodeLookupTask.cc',
        'src/DHTNodeLookupEntry.cc',
        'src/DHTBucketTree.cc',
        'src/DHTNodeLookupTaskCallback.cc',
        'src/DHTPeerLookupTaskCallback.cc',
        'src/DHTAbstractTask.cc',
        'src/DHTPingTask.cc',
        'src/DHTTaskQueueImpl.cc',
        'src/DHTTaskExecutor.cc',
        'src/DHTBucketRefreshTask.cc',
        'src/DHTPeerLookupTask.cc',
        'src/DHTSetup.cc',
        'src/DHTTaskFactoryImpl.cc',
        'src/DHTInteractionCommand.cc',
        'src/DHTPeerAnnounceEntry.cc',
        'src/DHTPeerAnnounceStorage.cc',
        'src/DHTTokenTracker.cc',
        'src/DHTGetPeersCommand.cc',
        'src/DHTTokenUpdateCommand.cc',
        'src/DHTBucketRefreshCommand.cc',
        'src/DHTPeerAnnounceCommand.cc',
        'src/DHTReplaceNodeTask.cc',
        'src/DHTEntryPointNameResolveCommand.cc',
        'src/DHTRoutingTableSerializer.cc',
        'src/DHTRoutingTableDeserializer.cc',
        'src/DHTAutoSaveCommand.cc',
        'src/DHTRegistry.cc',
        'src/PeerAddrEntry.cc',
        'src/InitiatorMSEHandshakeCommand.cc',
        'src/ReceiverMSEHandshakeCommand.cc',
        'src/MSEHandshake.cc',
        'src/BtLeecherStateChoke.cc',
        'src/BtSeederStateChoke.cc',
        'src/RangeBtMessage.cc',
        'src/IndexBtMessage.cc',
        'src/ZeroBtMessage.cc',
        'src/RangeBtMessageValidator.cc',
        'src/IndexBtMessageValidator.cc',
        'src/ExtensionMessageRegistry.cc',
        'src/bittorrent_helper.cc',
        'src/BtStopDownloadCommand.cc',
        'src/PriorityPieceSelector.cc',
        'src/LpdMessageDispatcher.cc',
        'src/LpdMessageReceiver.cc',
        'src/LpdMessage.cc',
        'src/LpdReceiveMessageCommand.cc',
        'src/LpdDispatchMessageCommand.cc',
        'src/bencode2.cc',
        'src/BencodeParser.cc',
        'src/UDPTrackerClient.cc',
        'src/UDPTrackerRequest.cc',
        'src/NameResolveCommand.cc',
        'src/Metalinker.cc',
        'src/MetalinkEntry.cc',
        'src/MetalinkResource.cc',
        'src/MetalinkMetaurl.cc',
        'src/MetalinkParserController.cc',
        'src/MetalinkParserStateMachine.cc',
        'src/MetalinkParserState.cc',
        'src/MetalinkParserStateImpl.cc',
        'src/MetalinkParserStateV3Impl.cc',
        'src/MetalinkParserStateV4Impl.cc',
        'src/Metalink2RequestGroup.cc',
        'src/MetalinkPostDownloadHandler.cc',
        'src/metalink_helper.cc',
        'src/clock_gettime_osx.cc',
        'src/PollEventPoll.cc',
        'src/KqueueEventPoll.cc',
        'src/aria2api.cc',
        'src/KeepRunningCommand.cc',
        'src/ApiCallbackDownloadEventListener.cc',
      ],
      'dependencies': [
        'wslay',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/include/libxml2',
          'deps/wslay/lib/includes',
          'src/includes',
          'config/<(OS)/<(target_arch)'
        ],
      },
    },
    {
      'target_name': 'test',
      'type': 'executable',
      'dependencies': [ 'aria2' ],
      'sources': [ 'examples/libaria2ex.cc' ],
      'conditions': [
        [ 'OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'GCC_ENABLE_CPP_RTTI': 'YES',
            'MACOSX_DEPLOYMENT_TARGET': '10.8',
          },
        }],
      ],
    },
  ]
}