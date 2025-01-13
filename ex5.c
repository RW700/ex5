#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// main menu options:
#define MAIN_MENU_EXIT 4
#define WATCH_OPTION 1
#define ADD_PLAYLIST_OPTION 2
#define REMOVE_PLAY_OPTION 3
// watch specific playlist menu options:
#define SHOW_PLAYLIST_OPT 1
#define ADD_SONG_OPT 2
#define DELETE_SONG_OPT 3
#define SORT_PLAYLIST_OPT 4
#define PLAY_PLAYLIST_OPT 5
#define PLAYLIST_EXIT 6

#define MALLOC_ERROR 2

#define INF_STR_TITLE 1
#define INF_STR_ARTIST 2
#define INF_STR_LYRICS 3

#define SORT_YEAR 1
#define SORT_STREAMS_ASC 2
#define SORT_STREAMS_DESC 3
#define SORT_ALPHABETICAL 4

#define STRING_BUFFER 10

typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char* name;
    Song** songs;
    int songsNum;
    // use as linked list
    struct Playlist* next;
} Playlist;

//some recommendations for functions, you may implement however you want

void deleteSong() {

    printf("Song deleted successfully.\n");
}

void playSong(Playlist* head, int songToPlay) {
    printf("Now playing %s:\n", head->songs[songToPlay - 1]->title);
    printf("$ %s $\n", head->songs[songToPlay - 1]->lyrics);
    head->songs[songToPlay - 1]->streams++;
}

void getInfiniteStringSong(Song* song, int type) {

    int len = 0;
    char tempChar = 'a';
    char* tempStr = (char*)malloc(sizeof(char) * 2);
    if (tempStr == NULL) {
        printf("Memory allocation failed\n");
        exit(MALLOC_ERROR);
        return;
    }
    while (tempChar != '\n' && tempChar != '\r' && tempChar != '\0') {
        scanf("%c", &tempChar);
        // tempChar = getchar();
        if (tempChar == '\n' || tempChar == '\r' || tempChar == '\0') {
            tempStr[len] = '\0';
            if (type == INF_STR_TITLE) {
                song->title = (char*)realloc(tempStr, sizeof(char) * (len + 1));
            } else if (type == INF_STR_ARTIST) {
                song->artist = (char*)realloc(tempStr, sizeof(char) * (len + 1));
            } else if (type == INF_STR_LYRICS) {
                song->lyrics = (char*)realloc(tempStr, sizeof(char) * (len + 1));
            }
            return;
        }
        char* newPtr = (char*)realloc(tempStr, sizeof(char) * (len + 2)); // Allocate space for the new character and the null terminator
        if (newPtr == NULL) {
            printf("Memory reallocation failed\n");
            free(tempStr);
            exit(MALLOC_ERROR);
            return;
        }
        tempStr = newPtr;
        // tempStr = (char*)realloc(tempStr, sizeof(char) * (len + 2));
        tempStr[len] = tempChar;
        len += 1;
    }
}

void getInfiniteString(Playlist* head) {
    getchar();
    int len = 0;
    char tempChar = 'a';
    char* tempStr = (char*)malloc(sizeof(char) * 2);
    if (tempStr == NULL) {
        printf("Memory allocation failed\n");
        exit(MALLOC_ERROR);
        return;
    }
    while (tempChar != '\n' && tempChar != '\r' && tempChar != '\0') {
        scanf("%c", &tempChar);
        // tempChar = getchar();
        if (tempChar == '\n' || tempChar == '\r' || tempChar == '\0') {
            tempStr[len] = '\0';
            // head->name = (char*)realloc(tempStr, sizeof(char) * (len + 1));
            char* newTestPtr = (char*)realloc(tempStr, sizeof(char) * (len + 1));
            if (newTestPtr == NULL) {
                printf("Memory reallocation failed\n");
                free(tempStr);
                exit(MALLOC_ERROR);
            }
            head->name = newTestPtr;
            return;
        }
        // Allocate space for the new character and the null terminator
        char* newPtr = (char*)realloc(tempStr, sizeof(char) * (len + 2));
        if (newPtr == NULL) {
            printf("Memory reallocation failed\n");
            free(tempStr);
            exit(MALLOC_ERROR);
            return;
        }
        tempStr = newPtr;
        // tempStr = (char*)realloc(tempStr, sizeof(char) * (len + 2));
        tempStr[len] = tempChar;
        len += 1;
    }
}

Playlist* addPlaylist(Playlist* head, int havePlaylist) {
    if (head == NULL) {
        head = (Playlist*)malloc(sizeof(Playlist));
        if (head == NULL) {
            printf("Memory allocation failed\n");
            exit(2);
        }
        getInfiniteString(head);
        return head;
    }
    if (havePlaylist == 0) {
        getInfiniteString(head);
        return head;
    }

    Playlist* temp = head;
    for (int i = 1; temp->next != NULL; i++) {
        temp = temp->next;
    }
    temp->next = (Playlist*)malloc(sizeof(Playlist));
    if (temp->next == NULL) {
        printf("Memory allocation failed\n");
        exit(2);
    }
    getInfiniteString(temp->next);
    return head;


}

void addSongToPlaylist(Playlist* playlist, Song* newSong) {
    // Song** temp = (Song**)realloc(playlist->songs, ((sizeof(Song*)) * (playlist->songsNum + 1)));
    playlist->songs = (Song**)realloc(playlist->songs, ((sizeof(Song*)) * (playlist->songsNum + 1)));
    if (playlist->songs == NULL) {
        printf("Memory reallocation failed\n");
        // for (int i = 0; i < playlist->songsNum; i++) {
        //     free(playlist->songs[i]->title);
        //     free(playlist->songs[i]->artist);
        //     free(playlist->songs[i]->lyrics);
        //     free(playlist->songs[i]);
        // }
        free(playlist->songs);
        exit(MALLOC_ERROR);
    }
    // playlist->songs = temp;
    playlist->songs[playlist->songsNum] = newSong;
    playlist->songsNum++;
}

void removeSongFromPlaylist(Playlist* playlist, int songIndex) {
    if (songIndex < 0 || songIndex >= playlist->songsNum) {
        printf("Invalid song index\n");
        return;
    }

    // Free the memory allocated for the song
    free(playlist->songs[songIndex]->title);
    free(playlist->songs[songIndex]->artist);
    free(playlist->songs[songIndex]->lyrics);
    free(playlist->songs[songIndex]);

    // Shift the remaining songs to fill the gap
    for (int i = songIndex; i < playlist->songsNum - 1; i++) {
        playlist->songs[i] = playlist->songs[i + 1];
    }

    // Decrease the number of songs
    playlist->songsNum--;

    // Reallocate memory to shrink the array
    playlist->songs = (Song**)realloc(playlist->songs, sizeof(Song*) * playlist->songsNum);
    if (playlist->songs == NULL && playlist->songsNum > 0) {
        printf("Memory reallocation failed\n");
        exit(MALLOC_ERROR);
    }

    printf("Song deleted successfully.\n");
}

void printPlaylistSongs(Playlist* playlist) {
    if (playlist->songsNum == 0) {
        return;
    }
    for (int i = 0; i < playlist->songsNum; i++) {
        Song* song = playlist->songs[i];
        printf("%d. Title: %s\n", i+1, song->title);
        printf("   Artist: %s\n", song->artist);
        printf("   Released: %d\n", song->year);
        printf("   Streams: %d\n\n", song->streams);
    }
}

int printPlaylists(Playlist* head, int upTo) {

    // for each playlist, print the name
    printf("\t%d. %s\n", upTo, head->name);

    // if there is a next playlist, call the function recursively
    if (head->next != NULL) {
        return printPlaylists(head->next, upTo + 1);
    }

    // otherwise print out the exit option, and return it for quick access to what value is exit
    printf("\t%d. Back to main menu\n", upTo + 1);
    return upTo + 1;

}

void freeSong() {

}

// void freePlaylist(P) {
//
// }

void freeAllSongs() {

}

Playlist* freePlaylistNode(Playlist* head, int toRemoveIndex) {

    Playlist* temp = head;
    Playlist* prev = NULL;

    if (head == NULL) {
        return head;
    }
    // if toRemove is the first element:
    if (toRemoveIndex == 1) {
        head = temp->next;
        //free each song from temp, then free temp
        for (int i = 0; i < temp->songsNum; i++) {
            free(temp->songs[i]->title);
            free(temp->songs[i]->artist);
            free(temp->songs[i]->lyrics);
            free(temp->songs[i]);
        }
        free(temp);
        return head;
    }
    //otherwise need to find element to remove:
    for (int i = 1; temp != NULL && i < toRemoveIndex; i++) {
        prev = temp;
        temp = temp->next;
    }
    if (temp != NULL) {
        prev->next = temp->next;
        //free each song from temp, then free temp
        for (int i = 0; i < temp->songsNum; i++) {
            free(temp->songs[i]->title);
            free(temp->songs[i]->artist);
            free(temp->songs[i]->lyrics);
            free(temp->songs[i]);
        }
        free(temp->name);
        free(temp);
    }
    return head;

}

void freeLastPlaylistNode(Playlist* head) {

    if (head == NULL) {
        return;
    }
    //free each song from temp, then free temp
    if (head->songsNum > 0) {
        for (int i = 0; i < head->songsNum; i++) {
            free(head->songs[i]->title);
            free(head->songs[i]->artist);
            free(head->songs[i]->lyrics);
            free(head->songs[i]);
        }
    }
    free(head->songs);
    free(head->name);
    free(head);
}


Playlist* removePlaylist(Playlist* head, int havePlaylist) {
    int returnOption = 1;
    int choice = 0;
    printf("Choose a playlist:\n");
    if (havePlaylist == 0 || head == NULL) {
        printf("\t1. Back to main menu\n");
    }
    else {
        returnOption = printPlaylists(head, 1);
    }
    int loopHolder = 1;
    while (loopHolder) {
        scanf("\n%d", &choice);

        if (choice == returnOption) {
            loopHolder = 0;
            return head;
        }
        // if they chose a number greater than return option - invalid input
        if (choice > returnOption || choice < 1) {
            printf("Invalid input\n");
        }
        else {
            // they chose a playlist, so, we gotta remove it:
            // removeRecursively(&head, choice);
            head = freePlaylistNode(head, choice);
            printf("Playlist deleted.\n");
            return head;
        }
    }
    return head;
}


void printPlaylistEditMenu() {
    printf("\t1. Show Playlist\n");
    printf("\t2. Add Song\n");
    printf("\t3. Delete Song\n");
    printf("\t4. Sort\n");
    printf("\t5. Play\n");
    printf("\t6. exit\n");
}

void printPlaylistsMenu() {
    printf("Please Choose:\n"); 
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");   
}



void sortPlaylist(Playlist* head, int sortType) {

    // we need to, by year, by streams or alphabetically (by title) sort
    // so
    // can use songnum to base sizes,
    // then, based on sortType, sort the songs using standard in-place sort
    switch (sortType) {
        case SORT_YEAR:
            for (int i = 0; i < head->songsNum - 1; i++) {
                for (int j = 0; j < head->songsNum - i - 1; j++) {
                    if (head->songs[j]->year > head->songs[j + 1]->year) {
                        Song *temp = head->songs[j];
                        head->songs[j] = head->songs[j + 1];
                        head->songs[j + 1] = temp;
                    }
                }
            }
            break;
        case SORT_STREAMS_ASC:
            for (int i = 0; i < head->songsNum - 1; i++) {
                for (int j = 0; j < head->songsNum - i - 1; j++) {
                    if (head->songs[j]->streams > head->songs[j + 1]->streams) {
                        Song *temp = head->songs[j];
                        head->songs[j] = head->songs[j + 1];
                        head->songs[j + 1] = temp;
                    }
                }
            }
            break;
        case SORT_STREAMS_DESC:
            for (int i = 0; i < head->songsNum - 1; i++) {
                for (int j = 0; j < head->songsNum - i - 1; j++) {
                    if (head->songs[j]->streams > head->songs[j + 1]->streams) {
                        Song *temp = head->songs[j];
                        head->songs[j] = head->songs[j + 1];
                        head->songs[j + 1] = temp;
                    }
                }
            }
            break;
        case SORT_ALPHABETICAL:
            for (int i = 0; i < head->songsNum - 1; i++) {
                for (int j = 0; j < head->songsNum - i - 1; j++) {
                    if (strcmp(head->songs[j]->title, head->songs[j + 1]->title) > 0) {
                        Song *temp = head->songs[j];
                        head->songs[j] = head->songs[j + 1];
                        head->songs[j + 1] = temp;
                    }
                }
            }
        break;
    }
}

void createSong(Song *newSong) {
    //clear input, then get inf string or int for each field
    getchar();
    printf("Enter song's details\n");
    printf("Title:\n");
    getInfiniteStringSong(newSong, INF_STR_TITLE);
    printf("Artist:\n");
    getInfiniteStringSong(newSong, INF_STR_ARTIST);
    printf("Year of release:\n");
    scanf("\n%d", &newSong->year);
    getchar();
    printf("Lyrics:\n");
    getInfiniteStringSong(newSong, INF_STR_LYRICS);
}

void watchPlaylistByNum(Playlist* head, int toWatchChoice, int upTo) {
    // recursively run through playlists, when choice == upTo - print info, otherwise call again
    if ( toWatchChoice == upTo) {
        // now the edit playlist options
        printf("playlist %s:\n", head->name);
        printPlaylistEditMenu();

        // they choose option
        int editPlaylistChoice = 0;
        int loopHolder = 1;
        while (loopHolder) {
            scanf("\n%d", &editPlaylistChoice);
            // if choice is exit, already gone, otherwise, case handling:
            switch (editPlaylistChoice) {
                case SHOW_PLAYLIST_OPT:
                    printPlaylistSongs(head);
                    printf("choose a song to play, or 0 to quit:\n");
                    int songChoice;
                    scanf("\n%d", &songChoice);
                    while (songChoice <= head->songsNum && songChoice > 0) {
                        if (songChoice <= 0 || songChoice > head->songsNum) {
                            break;
                        }
                        playSong(head, songChoice);
                        printf("choose a song to play, or 0 to quit:\n");
                        scanf("\n%d", &songChoice);
                    }
                    printPlaylistEditMenu();
                    break;
                case ADD_SONG_OPT:
                    Song* newSong = (Song*)malloc(sizeof(Song));
                    createSong(newSong);
                    addSongToPlaylist(head, newSong); //TODO VALGRIND - REMOVE AFT
                    free(newSong);
                    printPlaylistEditMenu();
                    break;
                case DELETE_SONG_OPT:
                    // first print out playlist:
                    printPlaylistSongs(head);
                    printf("choose a song to delete, or 0 to quit:\n");
                    int songDeleteChoice;
                    scanf("\n%d", &songDeleteChoice);
                    if (songDeleteChoice <= 0 || songDeleteChoice > head->songsNum) {
                        printPlaylistEditMenu();
                        break;
                    }
                    removeSongFromPlaylist(head, songDeleteChoice - 1);
                    printPlaylistEditMenu();
                    break;
                case SORT_PLAYLIST_OPT:
                    //TODO: implement sort playlist
                    printf("choose:\n");
                    printf("\t1. sort by year\n");
                    printf("\t2. sort by streams - ascending order\n");
                    printf("\t3. sort by streams - descending order\n");
                    printf("\t4. sort alphabetically\n");
                    int sortChoice;
                    scanf("\n%d", &sortChoice);
                    switch (sortChoice) {
                        case SORT_YEAR:
                            sortPlaylist(head, SORT_YEAR);
                            // sort by year
                            break;
                        case SORT_STREAMS_ASC:
                            // sort by streams - ascending order
                            sortPlaylist(head, SORT_STREAMS_ASC);
                            break;
                        case SORT_STREAMS_DESC:
                            // sort by streams - descending order
                            sortPlaylist(head, SORT_STREAMS_DESC);
                            break;
                        default:
                            // sort alphabetically
                            sortPlaylist(head, SORT_ALPHABETICAL);
                            break;
                    }
                    printf("sorted\n");

                    printPlaylistEditMenu();
                    break;
                case PLAY_PLAYLIST_OPT:
                    for (int i = 1; i <= head->songsNum; i++) {
                        playSong(head, i);
                    }
                    printPlaylistEditMenu();
                    break;
                case PLAYLIST_EXIT:
                    loopHolder = 0;
                    break;
                default:
                    printf("Invalid option\n");
                    break;
            }
        }
    } else {
        return watchPlaylistByNum(head->next, toWatchChoice, upTo + 1);
    }
}

void watchPlaylists(Playlist* head, int havePlaylist) {
    int toWatchChoice = 0;
    int returnOption = 1;

    printf("Choose a playlist:\n");
    if (havePlaylist == 0 || head == NULL) {
        printf("\t1. Back to main menu\n");
    }
    else {
        returnOption = printPlaylists(head, 1);
    }

    int loopHolder = 1;
    while (loopHolder) {
        scanf("\n%d", &toWatchChoice);

        if (toWatchChoice == returnOption) {
            // loopHolder = 0;
            return;
        }
        // if they chose a number greater than return option - invalid input
        if (toWatchChoice > returnOption || toWatchChoice < 1) {
            printf("Invalid input\n");
        }
        else {
            // they chose a playlist, so, we gotta return (so we exit after) and print the specific playlist:
            watchPlaylistByNum(head, toWatchChoice, 1);
            return watchPlaylists(head, havePlaylist);
        }
    }

}

void freeEverything(Playlist* head) {
    if (head == NULL) {
        free(head);
        return;
    }
    if (head->next == NULL) {
        //elem exists, next does not
        // freePlaylistNode(head, 1);
        return freeLastPlaylistNode(head);
    }
    return freeEverything(head->next);
}

int main() {

    // 'bool' to check if there are any playlists for edge case checking
    int havePlaylist = 0;
    // defines the head of the playlist that we'll assign playlists to once they initialize the data
    // Playlist head = {NULL, NULL, 0, NULL};

    Playlist *head = (Playlist*)malloc(sizeof(Playlist));
    int choice = 0;

    while(choice != MAIN_MENU_EXIT) {
        printPlaylistsMenu();
        scanf("\n%d", &choice);
        switch(choice) {
            case WATCH_OPTION:
                // printf("Choose a playlist:\n");
                watchPlaylists(head, havePlaylist);
                break;
            case ADD_PLAYLIST_OPTION:
                printf("Enter playlist's name:\n");
                head = addPlaylist(head, havePlaylist);
                havePlaylist = 1;
                break;
            case REMOVE_PLAY_OPTION:
                head = removePlaylist(head, havePlaylist);
                break;
            case 4:
                // printf("exit\n");
                // loopHolder = 0;
                freeEverything(head);
                break;
            default:
                printf("Invalid input\n");
                break;
        }
        // scanf("%*[^\n]");
        // getchar();

    }

    printf("Goodbye!\n");  
}
