Add to DefualtEngine.ini
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
[/Script/Engine.GameEngine]
+NetDriverDefinitions=(DefName="GameNetDriver",DriverClassName="OnlineSubsystemSteam.SteamNetDriver",DriverClassNameFallback="OnlineSubsystemUtils.IpNetDriver")

[OnlineSubsystem]
DefaultPlatformService=Steam

[OnlineSubsystemSteam]
bEnabled=true
SteamDevAppId=480
bInitServerOnClient=true

[/Script/OnlineSubsystemSteam.SteamNetDriver]
NetConnectionClassName="OnlineSubsystemSteam.SteamNetConnection"
---------------------------------------------------------------------------------------------------------------------------------------------------------------------

Add to DefaultGame.ini
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
[/Script/Engine.GameSession]
MaxPlayer = "Set desired number"