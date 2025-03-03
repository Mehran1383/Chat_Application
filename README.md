# Desktop chat application
---
## Details of implementaion design
### Communication with Server
The application uses provided APIs to send requests to the server and receive appropriate responses to be used in the messenger.

### API Structure
```
http://hostname:port/command?parameters
```
- **hostname**: Address of the server where the server code is running.
- **port**: Port for communication with the server.
- **command**: Commands requested from the server.
- **parameters**: Optional key-value pairs sent to the server with the request.

### Example Request
Registering a user with the username `Mehran` and password `1383`:
```
http://api.barafardayebehtar.ml:8080/signup?username=Mehran&password=1383
```
Responses are in JSON format, with `code` and `message` fields indicating the status and response message from the server.

### Response Codes
- **200**: Request successful.
- **404**: Request unsuccessful, information not found.
- **204**: Request unsuccessful, duplicate information.
- **401**: Request unsuccessful, incorrect information.

### Server Details
- **hostname**: `api.barafardayebehtar.ml`
- **port**: `8080`

### Request Format
```
http://api.barafardayebehtar.ml:8080/command?parameters
```

### Main Messenger Scenario
1. **Sign Up**: Register as a user.
2. **Login**: Obtain a unique token representing user identity.
3. **All Requests**: Sent with the unique token for user identification.
4. **Logout**: Token is set to `null` and a new token is assigned upon next login.

### Available APIs
| Command            | Parameters                                  | Description                                                              |
|--------------------|---------------------------------------------|--------------------------------------------------------------------------|
| signup             | username, password, firstname, lastname     | Register a new user.                                                     |
| login              | username, password                          | Log in a user and receive a unique token.                                |
| logout             | username, password                          | Log out a user and nullify their token.                                  |
| creategroup        | token, group_name, group_title              | Create a new group.                                                      |
| createchannel      | token, channel_name, channel_title          | Create a new channel.                                                    |
| getuserlist        | token                                       | Get the list of users the current user has chatted with.                 |
| getgrouplist       | token                                       | Get the list of groups the current user is a member of.                  |
| getchannellist     | token                                       | Get the list of channels the current user is a member of.                |
| getuserchats       | token, dst, date                            | Get the list of messages with a specific user.                           |
| getgroupchats      | token, dst, date                            | Get the list of messages in a specific group.                            |
| getchannelchats    | token, dst, date                            | Get the list of messages in a specific channel.                          |
| joingroup          | token, group_name                           | Join a specific group.                                                   |
| joinchannel        | token, channel_name                         | Join a specific channel.                                                 |
| sendmessageuser    | token, dst, body                            | Send a message to a specific user.                                       |
| sendmessagegroup   | token, dst, body                            | Send a message in a specific group.                                      |
| sendmessagechannel | token, dst, body                            | Send a message in a specific channel.                                    |

### Offline Message Access
The application stores all requested messages in categorized files to access the messenger information offline.

1. Application creates folders: `Users`, `Groups`, `Channels`.
2. For each user, group, and channel, applicaton creates respective files and save messages for offline access.

## Some features of project

In addition to sending and receiving messages, this program has these features :

---
1. A beautiful ui which makes this messenger more atractive

![Screenshot (603)](https://github.com/KassemKayall/Massenger/assets/134122361/b06dd1fa-2273-49b5-8d0d-bdeb44dbda63)
---
2. Ability to capture or select a picture for profile

![Screenshot (604)](https://github.com/KassemKayall/Massenger/assets/134122361/0f5c2aea-39a0-43d5-94c0-7cca7181243f)
---
3. Changing background theme

![Screenshot (605)](https://github.com/KassemKayall/Massenger/assets/134122361/3fd931cf-f34a-4a9e-ad83-a339a4598f47)
---
4. Changing font style and size

![Screenshot (606)](https://github.com/KassemKayall/Massenger/assets/134122361/0531c097-e2a1-492d-a152-9201488508ce)
---




