#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "jsmn.h"
//#include <time.h>
void urlencxlate(char *out_line, const char *in_line);

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}
int main ()
{
   char *env_value, *content_type, fname[128], 
        *in_line, *out_line, command[128];
   int content_length;
   FILE *tempfile;

   printf("Content-type: text/html\n\n");
   printf("<html>\n");
   printf("<head>\n");
   /*
   Handle error conditions or send success message to user
   */
   content_length = atoi(getenv("CONTENT_LENGTH"));
   env_value = getenv("CONTENT_TYPE");
   content_type = malloc (strlen(env_value) + 1);
   if (content_type == NULL)
   {
      printf("<title>Error Occurred</title>\n");
      printf("</head> <body>\n");
      printf("<p>Major failure #1; please notify the webmaster\n");
      printf("</p> </body> </html>\n");
      fflush(stdout);
      exit (2);
   }
      strcpy(content_type, env_value);
   if (content_length <= 0)
   {
      printf("<title>Error Occurred</title>\n");
      printf("</head> <body>\n");
      printf("<p>The form is empty; please enter some data!\n");
      printf("<br>\n"); 
      printf("Press the BACK key to return to the form.\n");
      printf("</p> </body> </html>\n");
      fflush(stdout);
      exit (0);
   }
   else if (strcmp(content_type, "application/json") != 0)
   {
      printf("<title>Content Error Occurred</title>\n");
      printf("</head> <body>\n");
      printf("<p>Internal error - invalid content type.  Please report\n");
      printf("<br>\n");
      printf("Press the BACK key to return to the form.\n");
      printf("</p> </body> </html>\n");
      fflush(stdout);
      exit (0);
   }

   in_line = malloc (content_length + 1);
   if (in_line == NULL)
   {
      printf("<title>Error Occurred</title>\n");
      printf("</head> <body>\n");
      printf("<p>Major failure #2; please notify the webmaster\n");
      printf("</p> </body> </html>\n");
      fflush(stdout);
      exit (2);
   }
   out_line = malloc (content_length + 1);
   if (out_line == NULL)
   {
      printf("<title>Error Occurred</title>\n");
      printf("</head> <body>\n");
      printf("<p>Major failure #3; please notify the webmaster\n");
      printf("</p> </body> </html>\n");
      fflush(stdout);
      exit (2);
   }
   gets(in_line);                                   /* read in form data */
   
   urlencxlate(out_line, in_line);                  /* convert */

   /* BEGIN jsonparse*/

   int i;
   int r;
   jsmn_parser p;
   jsmntok_t t[256]; /* We expect no more than 128 tokens */

   jsmn_init(&p);
   r = jsmn_parse(&p, out_line, strlen(out_line), t,
                  sizeof(t) / sizeof(t[0]));
   if (r < 0) {
      printf("Failed to parse JSON: %d\n", r);
      return 1;
   }  

   /* Loop over all keys of the root object */
   for (i = 1; i < r; i++) {
      //   printf(out_line);
      if (jsoneq(out_line, &t[i], "Engine") == 0) {
         /* We may use strndup() to fetch string value */
         printf("Engine: %.*s\n", t[i + 1].end - t[i + 1].start,
               out_line + t[i + 1].start);
         i++;
      } else if (jsoneq(out_line, &t[i], "first-year") == 0) {
         /* We may want to do strtol() here to get numeric value */
         printf("- First year: %.*s\n", t[i + 1].end - t[i + 1].start,
               out_line + t[i + 1].start);
         i++;
      } else if (jsoneq(out_line, &t[i], "last-year") == 0) {
         /* We may want to do strtol() here to get numeric value */
         printf("- Last year: %.*s\n", t[i + 1].end - t[i + 1].start,
               out_line + t[i + 1].start);
         i++;
      } else if (jsoneq(out_line, &t[i], "Comments") == 0) {
         printf("- Comments: %.*s\n", t[i + 1].end - t[i + 1].start,
            out_line + t[i + 1].start);
         i++;
      } else {
         printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
               out_line + t[i].start);
      }
   }

   /* END jsonparse*/
   /* BEGIN fileparse*/
      
      strcpy(fname, "/home/pi/embed/Embeddedopdracht2/html/models.json");
      tempfile = fopen (fname, "rw");     /* create temporary file */
      if (tempfile == NULL)              /* error - didn't create file */
      {
         printf("Internal failure #1 please report %d\n", errno);
         printf("</p> </body> </html>\n");
         fflush(stdout);
         exit (1);
      }
      // fprintf(tempfile, "%s", out_line);
      
      const int MAX_SIZE = 1024;
      char buf[MAX_SIZE];
      char output[4096];
      i = 0;
      while(i < 1024){
         fgets(buf, MAX_SIZE, tempfile);
         // printf(buf);
         *output = strcat(output, buf);
         // printf("<br>");
         if(strcmp(buf,"]")== 0){
            i = 1024;
         }
         i++;
      // cerr << "Read error";
      // exit(1);
      }
      
      output[strlen(output)-1] = '\0';
      output[strlen(output)];
      fclose(tempfile);
      tempfile = fopen(fname, "w");
      
      *output = strcat(output, ",");
      *output = strcat(output, out_line);
      *output = strcat(output, "\n]");
      output[0]='[';
      printf(output);
      fputs(output, tempfile);
      fclose(tempfile);
      

   /*END fileparse*/
   printf("<title>Hello and welcome!</title>\n");
   printf("<title>Form Submitted</title>\n");
   printf("</head> <body>\n");
   printf("<h1>Your Form Has Been Submitted</h1>\n");
   printf("Press the BACK key to return to the form.\n");
   printf("</p> </body> </html>\n");
   //remove (fname);                                  /* clean up */
   exit (0);
}
void urlencxlate(char *out_line, const char *in_line)
{
   int in_length, loop_index, out_index;
   in_length = strlen(in_line);
   for (loop_index = 0, out_index = 0; loop_index < in_length; 
        loop_index++)
{
      if (in_line[loop_index] == '%')               /* needs translation */
      {
        /* if your system uses signed characters, use strtol(). */
        /* You may want to apply validity 
           checking to the individual characters */
out_line[out_index] = strtoul(in_line+loop_index + 1, NULL, 16);
         out_index++;
         loop_index += 2;                     /* skip rest of hex value */
}
      else if (in_line[loop_index] == '+')    /* make a space */
      {
         out_line[out_index] = ' ';
         out_index++;
      }
      else if (in_line[loop_index] == '&')    /* make a newline */
      {
         out_line[out_index] = '\n';
         out_index++;
      }
      else                                    /* just copy */
      {
         out_line[out_index] = in_line[loop_index];
         out_index++;
      }
   }
   out_line[out_index] = '\0';                /* null terminate string */
}