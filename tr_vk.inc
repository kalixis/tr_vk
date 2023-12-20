/*

	About: VK v2.1
	Author: kalixis

*/

#if !defined _samp_included
	#error "Please include a_samp or a_npc before tr_vk"
#endif

#if !defined HTTP_ERROR_MALFORMED_RESPONSE
	#error "You must have a_http include in order to use this one."
#endif

#if defined _tr_vk_included_
	#endinput
#endif
#define _tr_vk_included_

/*

	Define const

*/

#if !defined TR_VK_URL
	#error Please define the URL of the vk script. Example: #define TR_VK_URL "example.com/vk.php"
#endif

#if !defined TR_VK_TOKEN
	#error Please define VK token to send messages. Example: #define TR_VK_TOKEN "a4a15d548494c1c5daea9812484ef2141ed649657faad41141e9d97d4b11c6cc4eb969aac5ec4d747951e"
#endif

#if !defined MAX_FUNCTION_NAME
	#define MAX_FUNCTION_NAME 32
#endif

#if !defined MAX_PLAYER_VK
	#define MAX_PLAYER_VK 64
#endif

#if !defined MAX_VK_SIZE
	#define MAX_VK_SIZE 512
#endif

/*

	Define functions

*/

#define VK_Create:%0(%1) \
	forward VC_%0(%1); \
	public VC_%0(%1)

#define VK_Show::%0(%1) \
	VC_%0(%1)

#define VK_Response:%0(%1) \
	forward VR_%0(%1); \
	public VR_%0(%1)

#define VK: #

#if !defined isnull
    #define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

/*

	Forwards

*/

forward OnPlayerVKScriptResponse(playerid, response_code, const data[]);

/*

	Vars

*/

static
	vk_player_function[MAX_PLAYERS][MAX_FUNCTION_NAME + 1 char];

/*

	Public functions

*/

stock VK_Send(playerid, const function[], const domain[], const message[])
{
	new
		info[MAX_VK_SIZE + 1];

	vk_player_function[playerid] = !"VR_";
	strcat(vk_player_function[playerid], function);
	format(info, sizeof(info), "domain=%s&message=%s&access_token="TR_VK_TOKEN"", domain, message);
	HTTP(playerid, HTTP_POST, TR_VK_URL, info, !"OnPlayerVKScriptResponse");
}

/*

	OnPlayerVKScriptResponse

*/

public OnPlayerVKScriptResponse(playerid, response_code, const data[])
{
	if (funcidx(vk_player_function[playerid]) != -1)
		CallLocalFunction(vk_player_function[playerid], !"iis", playerid, response_code, isnull(data) ? !"\1" : data);
	return 1;
}