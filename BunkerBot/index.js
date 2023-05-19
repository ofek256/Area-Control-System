

const Discord = require ('discord.js');

const prefix = '!';

const client = new Discord.Client({

    allowedMentions : {
        parse: [`users` , `roles`],
        replieUser: true,

    },
    intents: [

        "GUILDS",
        "GUILD_MESSAGES" , 
        "GUILD_PRESENCES",
        "GUILD_MEMBERS",
        "GUILD_MESSAGE_REACTIONS",

    ],
});

client.on("ready" , () => {
    console.log("BunkerBot is online!")
})


client.login("MTA2NDQ5NTIzNDA4NTU3MjYxOA.GWJSYs.To_TeL5-CpHIZwmm5KoS9ST8psfzfeEwr2MqC4")


