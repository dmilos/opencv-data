/*
 Free for use for commercial and other type of usage

*/
#include <array>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <bitset>

class Aruco
 {
  public:
    typedef double scalar_type;
    typedef std::array< scalar_type, 2> uv_type;

    typedef std::uint64_t pattern_type;
    typedef std::ostream ostream_type;
    typedef std::array< std::uint8_t, 2> board_type;

    scalar_type m_square = 6;
    pattern_type m_pattern;
    board_type m_board = {4,4};

   uv_type size()const{ return { m_square * (m_board[0]+2), m_square * (m_board[1]+2) };  }

  public:
   int value( int x, int y )
    {
      return ( m_pattern & ( (std::uint64_t)1 << ( m_board[0]*y+x ) )  )? 1 : 0;
    }
   void print( ostream_type & ofs )
     {
      ofs << "<g "
          <<        " transform=\"translate( " << m_square << " " << m_square << ")\" "
      << ">"<< std::endl;

      std::uint64_t index= m_pattern;

      ofs << "<!-- 0b";
      for( int y=0; y< m_board[1]; ++y )
       {
        for( int x=0; x< m_board[0]; ++x )
         {
          ofs << value( x, y );
         }
       }
      ofs << "-->" << std::endl;

      ofs << "<text fill=\"rgb(160,160,160)\""
          <<        " transform=\"translate( " << (-5) << " " << (-7.5) << ")\" "
          <<"  font-size=\"" << (4) << "\" >0x";
      ofs << std::setbase( 16 ) << index << std::setbase( 10 ) ; ofs << "</text>" << std::endl;

    for( int y=-1; y< m_board[1]+1; ++y ){
     for( int x=-1; x< m_board[0]+1; ++x )
      {
       std::string color;
       if( -1 == x ) color="black";
       if( -1 == y ) color="black";
       if( m_board[0] == x ) color="black";
       if( m_board[1] == y ) color="black";
       if( ( 0<= x ) && (x<m_board[0]) && ( 0<= y ) && (y<m_board[1]) )
        {
         if( true )
          {
           switch(value( x, y ) )
            {
             case( 0 ): color = "black"; break;
             case( 1 ): color = "white"; break;
            }
          }
        }

       ofs <<  "<rect width=\"" << m_square << "\" height=\"" << m_square << "\" "
       << "x=\"" << x*m_square  << "\" "
       << "y=\"" << y*m_square  << "\" "

       << " style=\"fill:"
       << color
       << ";\" "
       << " />";
      }
      ofs << std::endl;
    }


   ofs << "</g>"<< std::endl;
   }
 };

class Page
 {
  public:
    typedef double scalar_type;
    typedef Aruco aruco_type;
    typedef std::array<scalar_type, 2 > uv_type;
    typedef std::string color_type;

    typedef std::array< std::uint8_t, 2> board_type;

    typedef std::array<Aruco, 4 > aruco_vector_type;
    typedef std::ostream ostream_type;
    typedef std::vector< color_type > table_type;

  public:
    aruco_vector_type m_av;
    board_type m_board = { 6,16};
    table_type m_table={"black","yellow","green","blue","red","cyan","magenta","yellow","aqua" };
    uv_type    m_canvas={ 210, 297};
    uv_type    m_margin={ 20, 20};
    scalar_type m_square = -1;
   Page( ){}

   void print( ostream_type & ofs )
    {
    uv_type adjust;
    if( -1 == m_square )
     {
      auto w = (m_canvas[0] - 2 * m_margin[0]  - 2*m_av[0].size()[0] )/m_board[0];
      auto h = (m_canvas[1] - 2 * m_margin[1]  - 2*m_av[1].size()[1] )/m_board[1];
      if( w < h )
       {
        m_square = w;
        adjust[0] = 0;
        adjust[1] = ( m_canvas[1] - 2 * m_av[1].size()[1] - m_square * m_board[1]) /2 -  m_margin[1];
       }
      else
       {
        m_square = h;
        adjust[0] = ( m_canvas[0] - 2 * m_av[0].size()[0] - m_square * m_board[0]) /2 -  m_margin[0];
        adjust[1] = 0;
       }
     }

    uv_type lo = { adjust[0] + m_margin[0] + m_av[0].size()[0], adjust[1] + m_margin[1]  + m_av[0].size()[1] };
    uv_type hi = { lo[0]+ m_board[0] * m_square, lo[1] + m_board[1] * m_square };


     ofs <<"  <svg height=\""<< m_canvas[1] << "mm\" width=\""<< m_canvas[0] << "mm\" >" << std::endl;
     ofs <<"  <sodipodi:namedview id=\"base\" inkscape:document-units=\"mm\" />" << std::endl;
     ofs <<"  <g  transform=\"translate(0 0) scale( 3.7795277 )\">" << std::endl;

       ofs <<"    <g  transform=\" translate(" << lo[0]  << " " << hi[1] << ")  \">" << std::endl;
       ofs <<"        <g  transform=\"rotate(+90)\">" << std::endl;
         m_av[0].print( ofs );
       ofs <<"        </g>" << std::endl;
       ofs <<"    </g>" << std::endl;

       ofs <<"  <g  transform=\"translate("<< hi[0]  << " " << hi[1]  << ") \">" << std::endl;
         m_av[1].print( ofs );
       ofs <<"       </g>" << std::endl;

       ofs <<"  <g  transform=\"translate(" << lo[0] << " " << lo[1] << ") \">" << std::endl; //rotate(-180)
       ofs <<"        <g  transform=\"rotate(+180)\">" << std::endl;
         m_av[2].print( ofs );
       ofs <<"        </g>" << std::endl;
       ofs <<"    </g>" << std::endl;

       ofs <<"  <g  transform=\"translate(" << hi[0] << " " << lo[1] << ")\">" << std::endl;//  rotate(-90)
       ofs <<"        <g  transform=\"rotate(-90)\">" << std::endl;
         m_av[3].print( ofs );
       ofs <<"        </g>" << std::endl;
       ofs <<"    </g>" << std::endl;

      ofs <<"  <g "
               << " transform=\"  translate(" << lo[0] << " " << hi[1] << ") scale( +1 -1 )\" "
               << ">" << std::endl;
      for( int y = 0; y< m_board[1]; ++y )
       {
        for( int x = 0; x< m_board[0]; ++x )
         {
          ofs <<  "<rect width=\"" << m_square << "\" height=\"" << m_square << "\" "
          << "x=\"" << x * m_square  << "\" "
          << "y=\"" << y * m_square << "\" "
          << " style=\"fill:"
          << ( ( ( y*m_board[0] + x < m_table.size() ) ) ? m_table[  y*m_board[0] + x ] : "gray"  )
          << ";\" "
          << " />";
          ofs << std::endl;
         }
       }
       ofs <<"       </g>" << std::endl;

      ofs << "<text fill=\"rgb(160,160,160)\""
          <<        " transform=\"translate( " << lo[0] - m_av[2].size( )[0] << " " << lo[1] +10 << ")\" "
          <<"  style=\"font-size:" << (4) << ";" << "font-family:'Courier New'"
                "\" >" << std::endl;
        for( auto const& color: m_table )
         {
          ofs << "<tspan sodipodi:role=\"line\">" << color << "</tspan>" << std::endl;
         }

        ofs << "</text>" << std::endl;

          ofs << std::endl;

     ofs <<"       </g>" << std::endl;
     ofs <<"  </svg>" << std::endl;

     ofs << "<!--" << std::endl;
     ofs << "name " <<  "some-name" << std::endl;
     ofs << "nbits " <<  m_av[0].m_board[0] * m_av[0].m_board[1]<< std::endl;
     ofs <<  std::setw( m_av[0].m_board[0] * m_av[0].m_board[1]  ) << std::setfill('0') << std::setbase( 2 ) << m_av[0].m_pattern << std::endl;
     ofs <<  std::setw( m_av[0].m_board[0] * m_av[0].m_board[1]  ) << std::setfill('0') << std::setbase( 2 ) << m_av[1].m_pattern << std::endl;
     ofs <<  std::setw( m_av[0].m_board[0] * m_av[0].m_board[1]  ) << std::setfill('0') << std::setbase( 2 ) << m_av[2].m_pattern << std::endl;
     ofs <<  std::setw( m_av[0].m_board[0] * m_av[0].m_board[1]  ) << std::setfill('0') << std::setbase( 2 ) << m_av[3].m_pattern << std::endl;
     ofs << "-->" << std::endl;
    }


 };

typedef std::array<std::uint8_t,3> color_t;

void BW( std::array<std::uint64_t, 4 > const& pattern )
 {
  Page page;

    page.m_board = { 1, 2};
    page.m_canvas={ 210, 297};
    page.m_margin={ 20, 20};
    page.m_square = -1;

    page.m_table.clear();
    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 };
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 };
    page.m_av[2].m_pattern = pattern[2]; page.m_av[2].m_board = { 5, 5 };
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 };

    page.m_table={"black","white" };

  page.print( std::ofstream("A4-BW.svg") );

 }

void EIGHT( std::array<std::uint64_t, 4 > const& pattern )
 {
  Page page;

    page.m_board = { 2, 4};
    page.m_canvas={ 210, 297};
    page.m_margin={ 20, 20};
    page.m_square = -1;

    page.m_table.clear();
    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 };
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 };
    page.m_av[2].m_pattern = pattern[2]; page.m_av[2].m_board = { 5, 5 };
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 };

    page.m_table={"black","white", "red", "cyan", "green", "magenta", "blue", "yellow" };

  page.print( std::ofstream("A4-EIGHT.svg") );

 }

void gray06( std::array<std::uint64_t, 4 > const& pattern )
 {
  Page page;

    page.m_board = { 2, 3};
    page.m_canvas={ 210, 297};
    page.m_margin={ 20, 20};
    page.m_square = -1;

    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 };
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 };
    page.m_av[2].m_pattern  = pattern[2]; page.m_av[2].m_board = { 5, 5 };
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 };

    page.m_table.clear();
    page.m_table={"#000000", "#404040",
                  "#808080", "#c0c0c0",
                   "#ffffff",  "#ffffff" };

  page.print( std::ofstream("A4-gray06.svg") );
 }

void gray32( std::array<std::uint64_t, 4 > const& pattern )
 {
  Page page;

    page.m_board = { 4, 8};
    page.m_canvas={ 210, 297};
    page.m_margin={ 20, 20};
    page.m_square = -1;

    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 };
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 };
    page.m_av[2].m_pattern = pattern[2]; page.m_av[2].m_board = { 5, 5 };
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 };

    page.m_table.clear();
    page.m_table={"#000000", "#080808",
                  "#101010", "#181818",
                  "#202020", "#383838",
                  "#404040", "#484848",
                  "#505050", "#585858",
                  "#606060", "#686868",
                  "#707070", "#787878",
                  "#808080", "#888888",
                  "#909090", "#989898",
                  "#a0a0a0", "#a8a8a8",
                  "#b0b0b0", "#b8b8b8",
                  "#c0c0c0", "#c8c8c8",
                  "#d0d0d0", "#d8d8d8",
                  "#e0e0e0", "#e8e8e8",
                  "#f0f0f0", "#f8f8f8", "#ffffff", "#ffffff" };

  page.print( std::ofstream("A4-gray32.svg") );

 }


void red32D( std::array<std::uint64_t, 4 > const& pattern  )
 {
  Page page;

    page.m_board = { 4, 8};
    page.m_canvas={ 210, 297};
    page.m_margin={ 20, 20};
    page.m_square = -1;

    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 };
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 };
    page.m_av[2].m_pattern = pattern[2];  page.m_av[2].m_board = { 5, 5 };
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 };

    page.m_table.clear();
    page.m_table={           "#080000",
                  "#100000", "#180000",
                  "#200000", "#280000",
                  "#300000", "#380000",
                  "#400000", "#480000",
                  "#500000", "#580000",
                  "#600000", "#680000",
                  "#700000", "#780000",
                  "#800000", "#880000",
                  "#900000", "#980000",
                  "#a00000", "#a80000",
                  "#b00000", "#b80000",
                  "#c00000", "#c80000",
                  "#d00000", "#d80000",
                  "#e00000", "#e80000",
                  "#f00000", "#f80000",
                             "#ff0000" };

  page.print( std::ofstream("A4-red32D.svg") );
 }

void red32L( std::array<std::uint64_t, 4 > const& pattern  )
 {
  Page page;

    page.m_board = { 4, 8};
    page.m_canvas={ 210, 297};
    page.m_margin={ 20, 20};
    page.m_square = -1;

    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 };
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 };
    page.m_av[2].m_pattern = pattern[2];  page.m_av[2].m_board = { 5, 5 };
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 };

    page.m_table.clear();
    page.m_table={"#ff0000", "#ff0808",
                  "#ff1010", "#ff1818",
                  "#ff2020", "#ff3838",
                  "#ff4040", "#ff4848",
                  "#ff5050", "#ff5858",
                  "#ff6060", "#ff6868",
                  "#ff7070", "#ff7878",
                  "#ff8080", "#ff8888",
                  "#ff9090", "#ff9898",
                  "#ffa0a0", "#ffa8a8",
                  "#ffb0b0", "#ffb8b8",
                  "#ffc0c0", "#ffc8c8",
                  "#ffd0d0", "#ffd8d8",
                  "#ffe0e0", "#ffe8e8",
                  "#fff0f0", "#fff8f8", "#ffffff", "#ffffff" };

  page.print( std::ofstream("A4-red32L.svg") );
 }

char number2hex( int number )
 {
  return "0123456789ABCDEF"[number];
 }

void scale32D(  std::string const& fileName, color_t const& color, std::array<std::uint64_t, 4 > const& pattern  )
 {
  Page page;

    page.m_board = { 4, 8};
    page.m_canvas={ 210, 297};
    page.m_margin={ 20, 20};
    page.m_square = -1;

    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 };
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 };
    page.m_av[2].m_pattern = pattern[2];  page.m_av[2].m_board = { 5, 5 };
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 };

    page.m_table.clear();
    for( int parameter=8; parameter < 256; parameter += 8 )
     {
      std::string text="#000000";

      color_t current;
      current[0] = std::uint8_t( color[0] * (parameter/255.0) );
      current[1] = std::uint8_t( color[1] * (parameter/255.0) );
      current[2] = std::uint8_t( color[2] * (parameter/255.0) );

      text[1] = number2hex( current[0]/16 );  text[2] = number2hex( current[0]%16 ) ;
      text[3] = number2hex( current[1]/16 );  text[4] = number2hex( current[1]%16 ) ;
      text[5] = number2hex( current[2]/16 );  text[6] = number2hex( current[2]%16 ) ;

      page.m_table.push_back( text );
     }
    {
     std::string text="#000000";
     text[1] = number2hex( color[0]/16 );  text[2] = number2hex( color[0]%16 ) ;
     text[3] = number2hex( color[1]/16 );  text[4] = number2hex( color[1]%16 ) ;
     text[5] = number2hex( color[2]/16 );  text[6] = number2hex( color[2]%16 ) ;
     page.m_table.push_back( text );
    }

  page.print( std::ofstream( fileName ) );
 }

void scale64( std::string const& fileName, color_t const& color, std::array<std::uint64_t, 4 > const& pattern  )
 {
  Page page;

    page.m_canvas = { 297, 420 };
    page.m_board = { 6, 11 };

    page.m_margin={ 23, 23 };
    page.m_square = -1;

    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 }; page.m_av[0].m_square = 7;
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 }; page.m_av[1].m_square = 7;
    page.m_av[2].m_pattern = pattern[2];  page.m_av[2].m_board = { 5, 5 }; page.m_av[2].m_square = 7;
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 }; page.m_av[3].m_square = 7;

    page.m_table.clear();
    for( int parameter=0; parameter< 256; parameter += 8 )
       {
        std::string text="#000000";

        color_t current;
        current[0] = std::uint8_t( color[0] * (parameter/255.0) );
        current[1] = std::uint8_t( color[1] * (parameter/255.0) );
        current[2] = std::uint8_t( color[2] * (parameter/255.0) );

        text[1] = number2hex( current[0]/16 );  text[2] = number2hex( current[0]%16 ) ;
        text[3] = number2hex( current[1]/16 );  text[4] = number2hex( current[1]%16 ) ;
        text[5] = number2hex( current[2]/16 );  text[6] = number2hex( current[2]%16 ) ;

        page.m_table.push_back( text );
       }

    for( int parameter=0; parameter< 256; parameter += 8 )
       {
        std::string text="#000000";
        
        color_t current;
        current[0] = std::uint8_t( ( 255 - color[0] ) * (parameter/255.0) + color[0]);
        current[1] = std::uint8_t( ( 255 - color[1] ) * (parameter/255.0) + color[1]);
        current[2] = std::uint8_t( ( 255 - color[2] ) * (parameter/255.0) + color[2]);
        
        text[1] = number2hex( current[0]/16 );  text[2] = number2hex( current[0]%16 ) ;
        text[3] = number2hex( current[1]/16 );  text[4] = number2hex( current[1]%16 ) ;
        text[5] = number2hex( current[2]/16 );  text[6] = number2hex( current[2]%16 ) ;

        page.m_table.push_back( text );
       }

  page.print( std::ofstream(fileName) );
 }

void cube32( std::array<std::uint64_t, 4 > const& pattern  )
 {
  Page page;

    page.m_canvas={ 210, 297};
    page.m_margin={ 20, 20};
    page.m_square = -1;
    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 };
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 };
    page.m_av[2].m_pattern = pattern[2];  page.m_av[2].m_board = { 5, 5 };
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 };

    page.m_board = { 4, 7 };

    page.m_table.clear();
    for( int red=0; red< 256; red += 127 )
     for( int grn=0; grn< 256; grn+= 127 )
      for( int blu=0; blu< 256; blu+= 127 )
       {
        std::string color="#000000";
        color[1] = "0123456789ABCDEF"[red/16] ;  color[2] = "0123456789ABCDEF"[red%16] ;
        color[3] = "0123456789ABCDEF"[grn/16] ;  color[4] = "0123456789ABCDEF"[grn%16] ;
        color[5] = "0123456789ABCDEF"[blu/16] ;  color[6] = "0123456789ABCDEF"[blu%16] ;

        page.m_table.push_back( color );
       }

  page.print( std::ofstream("A4-cube32.svg") );
 }

void cube64( std::array<std::uint64_t, 4 > const& pattern  )
 {
  Page page;

    page.m_canvas = { 297, 420 };
    page.m_board = { 6, 11 };

    page.m_margin={ 23, 23 };
    page.m_square = -1;

    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 }; page.m_av[0].m_square = 7;
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 }; page.m_av[1].m_square = 7;
    page.m_av[2].m_pattern = pattern[2];  page.m_av[2].m_board = { 5, 5 }; page.m_av[2].m_square = 7;
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 }; page.m_av[3].m_square = 7;

    page.m_table.clear();
    for( int red=0; red< 256; red += 85 )
     for( int grn=0; grn< 256; grn+= 85 )
      for( int blu=0; blu< 256; blu+= 85 )
       {
        std::string color="#000000";
        color[1] = "0123456789ABCDEF"[red/16] ;  color[2] = "0123456789ABCDEF"[red%16] ;
        color[3] = "0123456789ABCDEF"[grn/16] ;  color[4] = "0123456789ABCDEF"[grn%16] ;
        color[5] = "0123456789ABCDEF"[blu/16] ;  color[6] = "0123456789ABCDEF"[blu%16] ;

        page.m_table.push_back( color );
       }

  page.print( std::ofstream("A3-cube64.svg") );
 }


void main()
 {
  BW(     { 0b0000111110001111010111100, 0b0000110110111010000101011, 0b0000100001101101101110110, 0b0000100001100111110001011 } );
  gray06( { 0b0000100001110011011001011, 0b0000100001110001101110110, 0b0000100001101111110001011, 0b0000100001101101101110110 } );
  gray32( { 0b0000100001110101001001111, 0b0000100001110011011001011, 0b0000100001110001101110110, 0b0000100001101111110001011 } );

  EIGHT(  { 0b0000100001111001101110110, 0b0000100001110101001001111, 0b0000100001110011011001011, 0b0000100001110001101110110 } );

  cube32( { 0b0000100001111011011001011, 0b0000100001111001101110110, 0b0000100001110101001001111, 0b0000100001110011011001011 } );
  cube64( { 0b0000100001111101001001111, 0b0000100001111011011001011, 0b0000100001111001101110110, 0b0000100001110101001001111 } );

  red32L( { 0b0000100011010001110100111, 0b0000100011010001011110110, 0b0000100001111101001001111, 0b0000100001111011011001011 } );
 
  scale64( "A3-red64.svg",     {255,0,0}, { 0b0000100011010111001001110, 0b0000100011010111000001111, 0b0000100011010001110100111, 0b0000100011010001011110110 } );
  scale64( "A3-green64.svg",   {0,255,0}, { 0b0000100011011001011110110, 0b0000100011010111001001110, 0b0000100011010111000001111, 0b0000100011010001110100111 } );
  scale64( "A3-blue64.svg",    {0,0,255},   { 0b0000100011011001110100111, 0b0000100011011001011110110, 0b0000100011010111001001110, 0b0000100011010111000001111 } );

  scale64( "A3-cyan64.svg",    { 0, 255,255}, { 0b0000100011011111000001111, 0b0000100011011001110100111, 0b0000100011011001011110110, 0b0000100011010111001001110 } );
  scale64( "A3-magenta64.svg", { 255, 0,255}, { 0b0000100011011111001001110, 0b0000100011011111000001111, 0b0000100011011001110100111, 0b0000100011011001011110110 } );
  scale64( "A3-yellow64.svg",  { 255, 255,0}, { 0b0000100011100001110100111, 0b0000100011011111001001110, 0b0000100011011111000001111, 0b0000100011011001110100111 } );


  scale32D(  "A4-redD.svg", { 255, 0, 0}, { 0b0000100011010001011110110, 0b0000100001111101001001111, 0b0000100001111011011001011, 0b0000100001111001101110110 } );

  scale32D( "A4-greenD.svg", { 0, 255, 0}, {   0b0000100011100111010001011, 0b0000100011100001110100111, 0b0000100011011111001001110, 0b0000100011011111000001111 } );
  scale32D( "A4-blueD.svg",  { 0, 0, 255}, {   0b0000100011100111011001010, 0b0000100011100111010001011, 0b0000100011100001110100111, 0b0000100011011111001001110 } );
  scale32D( "A4-cyanD.svg",     {0,255,255}, {   0b0000100011101001110100111, 0b0000100011100111011001010, 0b0000100011100111010001011, 0b0000100011100001110100111 } );
  scale32D( "A4-magentaD.svg",  {255,0,255}, {   0b0000100011101111010001011, 0b0000100011101001110100111, 0b0000100011100111011001010, 0b0000100011100111010001011 } );
  scale32D( "A4-yellowD.svg",   {255,255,0}, {   0b0000100011101111011001010, 0b0000100011101111010001011, 0b0000100011101001110100111, 0b0000100011100111011001010 } );

  // TODO blue32D( {    TODO, TODO, TODO, TODO } );
  // TODO cyan32D( {    TODO, TODO, TODO, TODO } );
  // TODO yellow32D( {  TODO, TODO, TODO, TODO } );
  // TODO magenta32D( { TODO, TODO, TODO, TODO } );
 }