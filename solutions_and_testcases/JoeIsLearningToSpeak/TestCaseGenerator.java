
import java.util.*;
import java.io.*;

public class TestCaseGenerator
{
    private final int N;

    private final int numberOfWords;
    private final int numberOfKnownSentences;
    private final int numberOfUnknownSentences;
    private final int repetitionsOfUnknownSentences;

    private Random random;

    private Vector<String>  dictionary;


    public TestCaseGenerator( int numberOfWords,
                              int numberOfKnownSentences,
                              int numberOfUnknownSentences,
                              int repetitionsOfUnknownSentences )
    {
        random = new Random();
        N = 2+random.nextInt(4);

        dictionary = new Vector<String>();

        if ( numberOfWords < 1 )
            numberOfWords = 100 + random.nextInt( 500000-100 );

        this.numberOfWords = numberOfWords;
        this.numberOfKnownSentences = numberOfKnownSentences;
        this.numberOfUnknownSentences = numberOfUnknownSentences;
        this.repetitionsOfUnknownSentences = repetitionsOfUnknownSentences;

        for( int i=0; i < numberOfWords; i++ ) {
            dictionary.add( generateNewWord() );
        }
    }

    private String generateNewWord()
    {
        final int maxLetter = 'z'-'a'+1;
        char A[] = new char [ 1+random.nextInt(8) ];

        for( int i=0; i < A.length; i++ ) {
            A[i] = (char)('a' + random.nextInt( maxLetter ));
        }

        return new String( A );
    }
    private String obtainWord()
    {
        return dictionary.get( random.nextInt( numberOfWords ) );
    }
        
    public void toStream( PrintStream ps )
    {
        ps.println( N );
        generatePreviousKnowledge( ps );
        ps.println();
        ps.println('*');
        generateConversation( ps );
        ps.println();
        ps.println('#');
    }

    private void generateSentences( int numberOfSentences, int repetitions, PrintStream ps )
    {
        List<String> sentences = new LinkedList<String>();
        String currentSentence = "";

        while( --numberOfSentences >= 0 ) {

            int numberOfSubsentences = 1 + random.nextInt( 6 );
            while( --numberOfSubsentences >= 0 ) {
                int numberOfWordsInSubsentence = 1 + random.nextInt( 20 );
                while( --numberOfWordsInSubsentence >= 0 ) {
                    currentSentence += " " + obtainWord();
                }
                if ( numberOfSubsentences > 0 ) {
                    if ( random.nextBoolean() ) {
                        currentSentence += " ,";
                        sentences.add( currentSentence );
                        currentSentence = "";
                    } else 
                        currentSentence += " , ";
                }
            }
            if ( random.nextBoolean() ) {
                currentSentence += " .";
                sentences.add( currentSentence );
                currentSentence = "";
            } else {
                currentSentence += " . ";
            }
        }

        for( int r=0; r < repetitions; r++ ) {
            for( String s : sentences ) ps.println( s );
        }
    }

    private void generatePreviousKnowledge( PrintStream ps )
    {
        if ( numberOfKnownSentences > 0 )
            generateSentences( numberOfKnownSentences, 1, ps );
        else
            generateSentences( 10 + random.nextInt( 1000 ), 1, ps );
    }

    private void generateConversation( PrintStream ps )
    {
        if ( numberOfUnknownSentences > 0 )
            generateSentences( numberOfUnknownSentences, repetitionsOfUnknownSentences, ps );
        else
            generateSentences( 100 + random.nextInt( 20000 ), repetitionsOfUnknownSentences, ps );
    }
}

/*
-Complexity of learnt sentences: 2
-Previous knowledge:
Joe is my name. I have a dog. My dog is a cinnamon coloured golden retriever.
The name of my father is Ben and the name of my mother is Linda.

-Conversation:
Hello Joe.
How are you?
Last day you told me: the name of my mother is Linda.
You also told me: the name of my father is Ben.
*/
