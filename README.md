# webserv

### This is a project in which we write our own HTTP server.

This is when i finally understand why a URL starts
with HTTP

The web server can accept a configuration file.

### In the configuration file, you can:
- Choose the port and host of each ’server’.
- Setup the server_names or not.
- Setup default error pages.
- Limit client body size.
- Setup routes with one or multiple of the following rules/configuration:
  - Define a list of accepted HTTP methods for the route.
  - Define a HTTP redirection.
  - Define a directory or a file from where the file should be searched (for example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet).
  - Turn on or off directory listing.
  - Set a default file to answer if the request is a directory.
  - Execute CGI based on certain file extension (for example .php).
  - Make the route able to accept uploaded files and configure where they should be saved.
