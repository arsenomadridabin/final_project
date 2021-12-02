# OSProject

- For authentication, use username and password from user_credential.txt and admin_credential.txt
- Authentication for superUser not working on classes server because of some trailing alphanumeric characters appended (Working in local machine).


- Compile Server program : gcc -o server server.c
- Compile SuperUser program : gcc -o superUser superUser.c
- Compile NormalUser program : gcc -o normalUser normalUser.c

- Run server program using ./server
- Run client program superuser using ./superUser
- Run client program normaluser using ./normalUser

# Super User commands
- sudo -create lion (Create User lion)
- sudo -create tiger (Creates User Tiger)
- sudo -delete lion (Delete user lion)
- touch random.txt (Create a sample text file form shell)
- sudo del random.txt (Deletes a file named random.txt)
- sudo fileown tiger.txt (Return owner of the file tiger.txt)
- sudo luf all (Returns all the available files from all user)
- sudo luf tiger (Return files owned by tiger)
- sudo listuser (Returns all availble users)

# Normal User Commands
- {user} listfile all (Returns all files)
- {user} upfile upload.txt (Uploads a file named upload.txt)
- {user} downfile upload.txt (Downloads a file named download.txt)


