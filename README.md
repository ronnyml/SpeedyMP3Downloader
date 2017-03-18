Speedy MP3 Downloader is a QT4 program to download MP3 Files in a fast way.

You need to store the MP3s info and links in a XML file and it should have the following structure:

```
<songs>  
<item>  
  <id>1a2b3c4d5e6f7g8h9i</id>  
  <name>Imagine</name>  
  <artist>The Beatles</artist>  
  <mp3>http://your-music-server.com/imagine-beatles.mp3</mp3>  
  <duration>184</duration>  
</item>  
</songs>
```

When you search for an artist name or song, the query will result in an XML file with the songs.
The program will parse this XML file and show you the items in a Table View.