-- Start of the configuration. This is the only node in the config file. 
-- The rest of them are sub-nodes
configuration=
{
	-- if true, the server will run as a daemon.
	-- NOTE: all console appenders will be ignored if this is a daemon
	daemon=false,

	logAppenders=
	{
		{
			name="console appender",
			type="coloredConsole",
			level=6
		},
		{
			name="file appender",
			type="file",
			level=6,
			fileName="/tmp/crtmpserver",
			fileHistorySize=10,
			fileLength=1024*256,
			singleLine=true
		}
	},
	
	applications=
	{
		rootDirectory="/usr/local/lib/crtmpserver/applications",
		
		
		{

			name="appselector",
			description="Application for selecting the rest of the applications",
			protocol="dynamiclinklibrary",
			validateHandshake=false,
			default=true,
			acceptors = 
			{
				{
					ip="0.0.0.0",
					port=1935,
					protocol="inboundRtmp"
				}
			
			}
		},
		{
			name="visio",
			description="Visio app",
			protocol="dynamiclinklibrary",
			aliases=
			{
				"live",
				"vod",
			},
			validateHandshake=false,
			acceptors = 
			{
				--[[
				{
					ip="0.0.0.0",
                                        port=6666,
                                        protocol="inboundLiveFlv",
                                        waitForMetadata=true,							},
				{
                                        ip="0.0.0.0",
                                        port=5544,
                                        protocol="inboundRtsp"
                                },
                                {
                                        ip="0.0.0.0",
                                        port=9999,
                                        protocol="inboundTcpTs"
                                },
				]]--
				
			},
			 mediaStorage = {
                                namedStorage1={
                                        description="Medias storage",
                                        mediaFolder="/usr/local/medias/",
                                        metaFolder="/tmp/metadata",
                                        
                                }
                        },
			--[[
			hosts_allowed ={
                                "mywebsite.com","www.mywebsite.com"
                                      }
			--]]

		},
	
	}
}

