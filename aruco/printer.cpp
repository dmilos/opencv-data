/*
 Free for use for commercial and other type of usage

*/
#include <array>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>

#include "z:\work\code\cpp\prj\github\color\work\src\color\color.hpp"

#if defined( color_a9f1aad2_e23c_4f7e_9dce_0f1c116f4968 )

#endif


typedef std::array< std::uint8_t, 3 > color8_type;
typedef std::array< double, 3 > colorDouble_type;

char number2hex( int number )
 {
  return "0123456789ABCDEF"[number];
 }

template< typename scalar_name >      //  _____/-----
 inline
 scalar_name
 ramp
  (
    scalar_name const& value
   ,scalar_name const& lower  = scalar_name( 0 )
   ,scalar_name const& higher = scalar_name( 1 )
  )
  {
   return ( (value) < lower ? lower : ( higher < (value) ? higher : (value) ) ) ;
  }

std::string to_text( color8_type const& c8 )
 {
  std::string result = "#ffffff";

  result[1] = "0123456789ABCDEF"[c8[0]/16] ;  result[2] = "0123456789ABCDEF"[c8[0]%16];
  result[3] = "0123456789ABCDEF"[c8[1]/16] ;  result[4] = "0123456789ABCDEF"[c8[1]%16];
  result[5] = "0123456789ABCDEF"[c8[2]/16] ;  result[6] = "0123456789ABCDEF"[c8[2]%16];

  return result;
 }

std::string to_text( colorDouble_type const& cD )
 {
  color8_type c8;
  // TODO c8 = cD;
  return to_text( c8 );
 }

void printTriangle
 (
   std::ostream & ofs
  ,std::array<std::array< double, 2>, 3 > const& point
 )
 {
  ofs << "<path ";
  ofs << "style=\"fill:#ffffff;\" "; //stroke:none;fill-opacity:1;
  ofs << "d=\"m " + std::to_string( point[0][0] )+ ", " + std::to_string( point[0][1] )+ " "
                  + std::to_string( point[1][0] )+ ", " + std::to_string( point[1][1] )+ " "
                  + std::to_string( point[2][0] )+ ", " + std::to_string( point[2][1] )+ " "
                  " z\" ";
  ofs << "/>";
 }

class Text
 {

  public:
    typedef double scalar_type;
    typedef std::array< scalar_type, 2> uv_type;
    typedef std::ostream ostream_type;

  public:
    Text( uv_type const& position, std::string const& content, std::string const& style )
     :m_position( position ), m_content(content), m_style(style)
    {}

  public:
    void print( ostream_type & ofs )
     {
     }

  private:
    uv_type      m_position;
    std::string  m_content;
    std::string  m_style;
 };


class Rectangle
 {
  public:
    typedef double scalar_type;
    typedef std::array< scalar_type, 2> uv_type;
    typedef std::ostream ostream_type;

  public:
    Rectangle( uv_type const& position, uv_type const& size, std::string const& style )
     :m_position( position ), m_size(size), m_style(style)
    {}

  public:
    void print( ostream_type & ofs )
     {
      ofs <<  "<rect "
      << "width=\"" << m_size[0] << "\" height=\"" << m_size[1] << "\" "
      << "x=\"" << m_position[0]  << "\" " << "y=\"" << m_position[1] << "\" "
      << " style=\"" << m_style << "\""
      << " />"<< std::endl;
     }

  private:
    uv_type  m_position, m_size;
    std::string m_style;
 };

class Aruco
 {
  public:
    typedef double scalar_type;
    typedef std::array< scalar_type, 2> uv_type;

    typedef std::uint64_t pattern_type;
    typedef std::ostream ostream_type;
    typedef std::array< std::uint16_t, 2> board_type;

    scalar_type m_square = 6;
    pattern_type m_pattern;
    board_type m_board = {4,4};

   uv_type size()const{ return { m_square * (m_board[0]+2), m_square * (m_board[1]+2) };  }

  public:
   int value( int x, int y )
    {
     int yp = (m_board[1] - 1) - y;
     int xp = (m_board[0] - 1) - x;
     return ( m_pattern & ( (std::uint64_t)1 << ( m_board[0]*yp+xp ) )  )? 1 : 0;
    }

   void print( ostream_type & ofs )
     {
      ofs << "<g "
          <<        " transform=\"translate( " << m_square << " " << m_square << ")\" "
      << ">"<< std::endl;

      std::uint64_t index = m_pattern;

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
          <<        " transform=\"translate( " << (-3) << " " << (-10) << ")\" "
          <<"  font-size=\"" << (6) << "\" >0x";
      ofs << std::setbase( 16 ) << index << std::setbase( 10 ) ; ofs << "</text>" << std::endl;

      // m -7,-7 0, -6.064253 -4.8804074, 6.1615763 z
      printTriangle( ofs, {  -0.95*m_square, -0.95*m_square , 0*0.65, - m_square *0.65, - m_square *0.65,   m_square *0.65 } );

      ofs << "<g>" ;
      for( int y=-1; y< m_board[1]+1; ++y )
       {
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
              switch( value( x, y ) )
               {
                case( 0 ): color = "black"; break;
                case( 1 ): color = "white"; break;
               }
             }
           }

         Rectangle( { x*m_square, y*m_square}, {m_square,m_square}, "fill:" + color + ";" ).print( ofs );
        }
       }
      ofs << "</g>" ;

      ofs << std::endl;
      ofs << "<text fill=\"rgb(160,160,160)\""
          <<        " transform=\"translate( " << m_square * ( m_board[1] ) * 0.5 << " " << m_square * ( m_board[1] + 1.5 ) << ")\" "
          <<"  font-size=\"" << m_square * 0.25 << "\" "
          <<"  style=\"" <<"text-anchor:middle;text-align:center;" << "\" "
          << " >"
          << "<tspan sodipodi:role=\"line\">"
          <<        m_square * ( m_board[0] +2 ) << "x" <<  m_square * ( m_board[1] +2 )
          << "(" << m_square * ( m_board[0] )    << "x" <<  m_square * ( m_board[1] ) << ")"
          << "[" << m_square                     << "x" <<  m_square                  << "]"
          << "mm"
          << "</tspan>"
          << "<tspan sodipodi:role=\"line\">"
          << m_board[0] +2<< "x" << m_board[1] +2 << "(" << m_board[0] << "x" << m_board[1] << ")"
          << "</tspan>"
       ;

      ofs << "</text>" << std::endl;

      ofs << std::endl;

      ofs << "</g>"<< std::endl;
     }
 };

#define TO10(a) ( ('0' <= (a) ) && ( (a) <= '9' ) ? (a)-'0' : (a)-'a' + 10 )


class Klin
 {
  public:
  typedef std::ostream ostream_type;
  double m_size=10;
  void print( ostream_type & ofs )
   {
    ofs <<"<g>" << std::endl;

    Rectangle( { 0*  m_size, 0*  m_size}, {m_size,m_size}, std::string("fill:") + "red" + ";" ).print( ofs );
    ofs << std::endl;

    Rectangle( { 1*  m_size, 0*  m_size}, {m_size,m_size}, std::string("fill:") + "green" + ";" ).print( ofs );
    ofs << std::endl;

    Rectangle( { 2*  m_size, 0*  m_size}, {m_size,m_size}, std::string("fill:") + "blue" + ";" ).print( ofs );
    ofs << std::endl;

    Rectangle( { 0*  m_size, 1*  m_size}, {m_size,m_size}, std::string("fill:") + "black" + ";" ).print( ofs );
    ofs << std::endl;

    Rectangle( { 1*  m_size, 1*  m_size}, {m_size,m_size}, std::string("fill:") + "gray" + ";" ).print( ofs );
    ofs << std::endl;

    Rectangle( { 2*  m_size, 1*  m_size}, {m_size,m_size}, std::string("fill:") + "white" + ";" ).print( ofs );
    ofs << std::endl;

    Rectangle( { 0*  m_size, 2*  m_size}, {m_size,m_size}, std::string("fill:")+ "cyan" + ";" ).print( ofs );
    ofs << std::endl;

    Rectangle( { 1*  m_size, 2*  m_size}, {m_size,m_size}, std::string("fill:") + "magenta" + ";" ).print( ofs );
    ofs << std::endl;

    Rectangle( { 2*  m_size, 2*  m_size}, {m_size,m_size}, std::string("fill:") + "yellow" + ";" ).print( ofs );
    ofs << std::endl;

    ofs <<"</g>" << std::endl;
    ofs << std::endl;

   }
 };

class Page
 {
  public:
    typedef double scalar_type;
    typedef std::uint16_t size_type;

    typedef Aruco aruco_type;
    typedef std::array<scalar_type, 2 > uv_type;
    typedef std::string color_type;

    typedef std::array< std::uint16_t, 2> board_type;

    typedef std::array<aruco_type, 4 > aruco_vector_type;
    typedef std::ostream ostream_type;
    typedef std::vector< color_type > table_type;

  private:
  void text( ostream_type & ofs, std::string const& content, color_type const& color, size_type const& size )
   {
    ofs << "<text "
               << " fill=\"" << color <<  "\""
               << " style=\"font-size:" << size << ";" << "font-family:'Courier New'\" "
              " >" << std::endl;
      ofs << content << std::endl;
      ofs << "</text>" << std::endl;
   }
  public:
    aruco_vector_type m_av;
    board_type m_board = { 6, 16 };
    table_type m_table={"black","yellow","green","blue","red","cyan","magenta","yellow","aqua" };
    uv_type    m_canvas={ 210, 297};
    uv_type    m_margin={ 20, 20};
    scalar_type m_square = -1;
    size_type m_legend = 17000;
    std::string m_title="";
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

     ofs <<"<g                              " << std::endl;
     ofs <<"   inkscape:label=\"Layer 1\"   " << std::endl;
     ofs <<"   inkscape:groupmode=\"layer\" " << std::endl;
     ofs <<"   id=\"layer1\">               " << std::endl;

     /// -----------------
     ofs <<"  <g  transform=\"translate(0 0) scale( 3.7795277 )\">" << std::endl; // main group. all in mm
     {
      Klin klin;
      {
         auto x = lo[0] + 0.5*(hi[0]-lo[0] ) -  klin.m_size * 1.5;
         auto y = lo[1] - m_av[0].size()[1];
         ofs <<"  <g  transform=\"translate(" << x  <<" " << y << ") scale( 1 )\">"; klin.print( ofs ); ofs <<"    </g>" << std::endl;
      }
      {
         auto x = lo[0] + 0.5*(hi[0]-lo[0] ) - klin.m_size * 1.5;
         auto y = hi[1] + m_av[0].size()[1] - klin.m_size * 3;
         ofs <<"  <g  transform=\"translate(" << x <<" " << y  << ") scale( 1 )\">"; klin.print( ofs ); ofs <<"    </g>" << std::endl;
      }
      {
         auto x = lo[0] - m_av[0].size()[0];
         auto y = lo[1] + 0.5*(hi[1]-lo[1] ) -  klin.m_size * 1.5;
         ofs <<"  <g  transform=\"translate(" << x <<" " << y << ") scale( 1 )\">"; klin.print( ofs ); ofs <<"    </g>" << std::endl;
      }
      {
         auto x = hi[0] + m_av[0].size()[0] - klin.m_size * 3;
         auto y = lo[1] + 0.5*(hi[1]-lo[1] ) -  klin.m_size * 1.5;
         ofs <<"  <g  transform=\"translate(" << x <<" " << y << ") scale( 1 )\">"; klin.print( ofs ); ofs <<"    </g>" << std::endl;
      }

     }


     {
      auto x = lo[0] + 0.1*(hi[0]-lo[0] );
      auto y = lo[1] - m_av[0].size()[1] + 7.5;
      ofs <<"  <g  transform=\"translate(" << x <<" " << y << ") scale( 1 )\">" << std::endl; text( ofs, m_title,  "#7f7f7f", 12 ); ofs <<"    </g>" << std::endl;
     }

     {
      std::string board = std::to_string( m_board[0] * m_board[1] ) + "=" + std::to_string( m_board[0] ) + "x" + std::to_string( m_board[1] );
      auto x = lo[0] + 0.1*(hi[0]-lo[0]);
      auto y = lo[1] - m_av[0].size()[1] + 7.5 + 12;
      ofs <<"  <g  transform=\"translate(" << x <<" " << y << ") scale( 1 )\">" << std::endl; text( ofs, board, "#a0a0a0", 5 ); ofs <<"    </g>" << std::endl;
     }
     {
      std::stringstream ss;
      ss << std::setprecision(4) << m_square << "mm";
      auto x = lo[0] + 0.1*(hi[0]-lo[0]);
      auto y = lo[1] - m_av[0].size()[1] + 7.5+12 + 5;
      ofs <<"  <g  transform=\"translate(" << x <<" " << y << ") scale( 1 )\">" << std::endl; text( ofs, ss.str(), "#a0a0a0", 5 ); ofs <<"    </g>" << std::endl;
     }

     {
      std::stringstream ss;
      ss << std::setprecision(4) << hi[0]-lo[0] << "x" <<  hi[1]-lo[1] << "mm";
      auto x = lo[0] + 0.1*(hi[0]-lo[0]);
      auto y = lo[1] - m_av[0].size()[1] + 7.5+12 + 5 + 5;
      ofs <<"  <g  transform=\"translate(" << x <<" " << y << ") scale( 1 )\">" << std::endl; text( ofs, ss.str(), "#a0a0a0", 5 ); ofs <<"    </g>" << std::endl;
     }


      ofs <<"  <g "
               << " transform=\"  translate(" << lo[0] << " " << hi[1] << ") scale( +1 -1 )\" "
               << ">" << std::endl;
      for( int y = 0; y< m_board[1]; ++y )
       {
        for( int x = 0; x< m_board[0]; ++x )
         {
          Rectangle( { x * m_square, y * m_square}, {m_square,m_square}, "fill:" + ( ( ( y*m_board[0] + x < m_table.size() ) ) ? m_table[  y*m_board[0] + x ] : "gray"  ) + ";" ).print( ofs );
          ofs << std::endl;
         }
       }

      ofs <<"       </g>" << std::endl;

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


      ofs << "<text fill=\"rgb(160,160,160)\""
          <<        " transform=\"translate( " << lo[0] - m_av[2].size( )[0] << " " << lo[1] +10 << ")\" "
          <<"  style=\"font-size:" << (2) << ";" << "font-family:'Courier New'""\" "
          <<">" << std::endl;
        size_type index = 0;
        for( auto const& color: m_table )
         {
         if( index == m_legend )
          {
           ofs << "<!-- ";
          }index++;
          ofs << "<tspan sodipodi:role=\"line\">";
          ofs << color;
          if( ( 7 == color.size() ) && ( '#' == color[0] ) )
           {
            ofs << "(" ;
            ofs << std::setw(3) << TO10(tolower(color[1])) * 16 + TO10(tolower(color[2])) ;
            ofs << "," ;
            ofs << std::setw(3) << TO10(tolower(color[3])) * 16 + TO10(tolower(color[4])) ;
            ofs << ","  ;
            ofs << std::setw(3) << TO10(tolower(color[5])) * 16 + TO10(tolower(color[6])) ;
           ofs << ")" ;
           }

          ofs << "</tspan>" << std::endl;
         }

        if( m_legend < index )
         {
          ofs << "-->";
         }
        ofs << "</text>" << std::endl;

      ofs << std::endl;

     ofs <<"       </g>" << std::endl;
     ofs <<"    </g><!-- Layer -->" << std::endl;
     ofs <<"  </svg>" << std::endl;

     ofs << "<!--" << std::endl;
     ofs << "name " <<  "some-name" << std::endl;
     ofs << "nbits " <<  m_av[0].m_board[0] * m_av[0].m_board[1]<< std::endl;
     ofs <<  std::setw( m_av[0].m_board[0] * m_av[0].m_board[1]  ) << std::setfill('0')<< std::setw(25) << std::bitset<25>( m_av[0].m_pattern ) << std::endl;
     ofs <<  std::setw( m_av[0].m_board[0] * m_av[0].m_board[1]  ) << std::setfill('0')<< std::setw(25) << std::bitset<25>( m_av[1].m_pattern ) << std::endl;
     ofs <<  std::setw( m_av[0].m_board[0] * m_av[0].m_board[1]  ) << std::setfill('0')<< std::setw(25) << std::bitset<25>( m_av[2].m_pattern ) << std::endl;
     ofs <<  std::setw( m_av[0].m_board[0] * m_av[0].m_board[1]  ) << std::setfill('0')<< std::setw(25) << std::bitset<25>( m_av[3].m_pattern ) << std::endl;
     ofs << "-->" << std::endl;


    }

 };

typedef std::array<std::uint16_t,3> color_t;
typedef std::array<double,3>        colorDouble_t;
typedef std::array<double,2>        coord2D_t;
typedef std::array<double,3>        coord3D_t;
typedef std::array<std::uint16_t,3> size2d_t;

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

void gray04( std::array<std::uint64_t, 4 > const& pattern )
 {
  Page page;

    page.m_board = { 2, 3 };
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
                  "#ffffff"  };

  page.print( std::ofstream("A4-gray04.svg") );
 }

void gray08( std::array<std::uint64_t, 4 > const& pattern )
 {
  Page page;

    page.m_board = { 2, 5 };
    page.m_canvas={ 210, 297};
    page.m_margin={ 20, 20};
    page.m_square = -1;

    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 };
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 };
    page.m_av[2].m_pattern  = pattern[2]; page.m_av[2].m_board = { 5, 5 };
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 };

    page.m_table.clear();
    page.m_table={ "#000000",
                   "#202020",
                   "#404040",
                   "#606060",
                   "#808080",
                   "#a0a0a0",
                   "#c0c0c0",
                   "#e0e0e0",
                   "#ffffff",
                   };

  page.print( std::ofstream("A4-gray08.svg") );
 }

void gray16( std::array<std::uint64_t, 4 > const& pattern )
 {
  Page page;

    page.m_board = { 3, 6 };
    page.m_canvas={ 210, 297};
    page.m_margin={ 20, 20};
    page.m_square = -1;

    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 };
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 };
    page.m_av[2].m_pattern  = pattern[2]; page.m_av[2].m_board = { 5, 5 };
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 };

    page.m_table.clear();
    page.m_table={ "#000000",
                   "#101010",
                   "#202020",
                   "#303030",
                   "#404040",
                   "#505050",
                   "#606060",
                   "#707070",
                   "#808080",
                   "#909090",
                   "#a0a0a0",
                   "#b0b0b0",
                   "#c0c0c0",
                   "#d0d0d0",
                   "#f0f0f0",
                   "#ffffff",
                   };

  page.print( std::ofstream("A4-gray16.svg") );
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

void cube27( std::array<std::uint64_t, 4 > const& pattern )
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
        if( 254 == red ) red = 255;
        if( 254 == grn ) grn = 255;
        if( 254 == blu ) blu = 255;
        page.m_table.push_back( to_text( color8_type( { (std::uint8_t)red, (std::uint8_t)grn, (std::uint8_t)blu } ) ) );
       }

  page.print( std::ofstream("A4-cube27.svg") );
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
    for( int red=0; red< 256; red+= 85 )
     for( int grn=0; grn< 256; grn+= 85 )
      for( int blu=0; blu< 256; blu+= 85 )
       {
        page.m_table.push_back( to_text( color8_type( { (std::uint8_t)red, (std::uint8_t)grn, (std::uint8_t)blu } ) ) );
       }

  page.print( std::ofstream("A3-cube64.svg") );
 }

void cubeG( std::string const& fileName, colorDouble_t const& step, std::array<std::uint64_t, 4 > const& pattern  )
 {
  Page page;

    page.m_canvas = { 297, 420 };

    page.m_margin={ 23, 23 };
    page.m_square = -1;

    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 }; page.m_av[0].m_square = 7;
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 }; page.m_av[1].m_square = 7;
    page.m_av[2].m_pattern = pattern[2];  page.m_av[2].m_board = { 5, 5 }; page.m_av[2].m_square = 7;
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 }; page.m_av[3].m_square = 7;

    page.m_table.clear();
    for( double red=0; red< 256; red+= step[0] )
     for( double grn=0; grn< 256; grn+= step[1] )
      for( double blu=0; blu< 256; blu+= step[2] )
       {
        page.m_table.push_back( to_text( color8_type( { (std::uint8_t)red, (std::uint8_t)grn, (std::uint8_t)blu } ) ) );
       }

    int space_w = ( int )( page.m_canvas[0] - 2* page.m_margin[0] - 2 * 7 * page.m_av[0].m_square );
    int space_h = ( int )( page.m_canvas[1] - 2* page.m_margin[1] - 2 * 7 * page.m_av[0].m_square );

    double unit = sqrt(  page.m_table.size() * ( space_w / (double)space_h ) );
    int h = int( unit + 0.5 );
    int w = int( unit * ( space_h / (double)space_w ) + 0.5 );
    if( h*w < page.m_table.size() ) w++;
    page.m_board = { (std::uint16_t)h, (std::uint16_t)w };

  page.m_legend = 100;
  page.print( std::ofstream(fileName) );
 }

void redALL( colorDouble_t const& step  ,std::array<std::uint64_t, 4 > const& pattern )
 {
  Page page;

    page.m_canvas = { 297, 420 };

    page.m_margin={ 23, 23 };
    page.m_square = -1;
    page.m_legend = 100;

    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 }; page.m_av[0].m_square = 7;
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 }; page.m_av[1].m_square = 7;
    page.m_av[2].m_pattern = pattern[2];  page.m_av[2].m_board = { 5, 5 }; page.m_av[2].m_square = 7;
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 }; page.m_av[3].m_square = 7;

    page.m_table.clear();
    for( double red=0; red< 256; red+= step[0] )
     for( double grn=0; grn< 256; grn+= step[1] )
      for( double blu=0; blu< 256; blu+= step[2] )
       {
        if( red <= grn ) continue;
        if( red <= blu ) continue;

        page.m_table.push_back( to_text( color8_type( { (std::uint8_t)red, (std::uint8_t)grn, (std::uint8_t)blu } ) ) );
       }
    {
     int space_w = ( int )( page.m_canvas[0] - 2* page.m_margin[0] - 2 * 7 * page.m_av[0].m_square );
     int space_h = ( int )( page.m_canvas[1] - 2* page.m_margin[1] - 2 * 7 * page.m_av[0].m_square );

     double unit = sqrt(  page.m_table.size() * ( space_w / (double)space_h ) );
     int h = int( unit + 0.5 );
     int w = int( unit * ( space_h / (double)space_w ) + 0.5 );
     if( h*w < page.m_table.size() ) w++;
     page.m_board = { (std::uint16_t)h, (std::uint16_t)w };
    }

   page.print( std::ofstream("A3-red-ALL.svg") );
 }


void lineA3
 (
   std::string const& fileName
  ,color_t const& start
  ,colorDouble_t const& step
  ,Aruco::board_type const& board
  ,std::array<std::uint64_t, 4 > const& pattern
 )
 {
  Page page;
    page.m_canvas = { 297, 420 };
    page.m_board = board;

    page.m_margin={ 21, 18 };
    page.m_square = -1;

    page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 }; page.m_av[0].m_square = 7;
    page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 }; page.m_av[1].m_square = 7;
    page.m_av[2].m_pattern = pattern[2];  page.m_av[2].m_board = { 5, 5 }; page.m_av[2].m_square = 7;
    page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 }; page.m_av[3].m_square = 7;

    page.m_legend = 100;

    page.m_table.clear();
    int total = board[0]*board[1];

    for( int index=0; index < total ; ++index )
     {
      color_t current;
      current[0] = std::uint8_t( ramp<double>( 0.01 + start[0] + index * step[0], 0, 255 ) );
      current[1] = std::uint8_t( ramp<double>( 0.01 + start[1] + index * step[1], 0, 255 ) );
      current[2] = std::uint8_t( ramp<double>( 0.01 + start[2] + index * step[2], 0, 255 ) );

      std::string color="#ffffff";
      color[1] = "0123456789ABCDEF"[current[0]/16] ;  color[2] = "0123456789ABCDEF"[current[0]%16] ;
      color[3] = "0123456789ABCDEF"[current[1]/16] ;  color[4] = "0123456789ABCDEF"[current[1]%16] ;
      color[5] = "0123456789ABCDEF"[current[2]/16] ;  color[6] = "0123456789ABCDEF"[current[2]%16] ;

      page.m_table.push_back( color );
     }

  page.print( std::ofstream( fileName ) );
 }

template < unsigned N_x, unsigned N_y >
void planeA4
 (
   std::string                   const& fileName
  ,colorDouble_t                 const& step
  ,Aruco::board_type             const& board
  ,std::array<std::uint64_t, 4 > const& pattern
 )
 {
  Page page;
  page.m_canvas = { 210, 297 };
  page.m_board = board;
  page.m_margin={ 20, 20 };
  page.m_square = -1;

  page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 }; page.m_av[0].m_square = 7;
  page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 }; page.m_av[1].m_square = 7;
  page.m_av[2].m_pattern = pattern[2];  page.m_av[2].m_board = { 5, 5 }; page.m_av[2].m_square = 7;
  page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 }; page.m_av[3].m_square = 7;

  page.m_legend = 70-12;

  page.m_table.clear();
  int total = board[0]*board[1];

  colorDouble_t current; current[0] = 0; current[1] = 0; current[2] = 0;
  for( int index=0; index < total  ; ++index )
   {
    if( 0 == N_x ) current[0] += step[0];
    if( 1 == N_x ) current[1] += step[1];
    if( 2 == N_x ) current[2] += step[2];

    if( 255 < current[0] ) { current[0] = 0; if( 0 == N_x ) current[N_y] += step[N_y]; }
    if( 255 < current[1] ) { current[1] = 0; if( 1 == N_x ) current[N_y] += step[N_y]; }
    if( 255 < current[2] ) { current[2] = 0; if( 2 == N_x ) current[N_y] += step[N_y]; }

    // TODO color::fix:overburn( current )
    color_t c16;
    c16[0] = std::uint16_t( current[0] + 0.01 );
    c16[1] = std::uint16_t( current[1] + 0.01 );
    c16[2] = std::uint16_t( current[2] + 0.01 );
    std::string txt="#ffffff";
    txt[1] = "0123456789ABCDEF"[c16[0]/16] ;  txt[2] = "0123456789ABCDEF"[c16[0]%16] ;
    txt[3] = "0123456789ABCDEF"[c16[1]/16] ;  txt[4] = "0123456789ABCDEF"[c16[1]%16] ;
    txt[5] = "0123456789ABCDEF"[c16[2]/16] ;  txt[6] = "0123456789ABCDEF"[c16[2]%16] ;

    page.m_table.push_back( txt );
   }

  page.print( std::ofstream( fileName ) );
 }

void MacbethA4( std::string const& fileName  ,std::array<std::uint64_t, 4 > const& pattern )
 {
  Page page;
  page.m_canvas = { 210, 297 };
  page.m_board = { 4, 6 };
  page.m_margin={ 20, 20 };
  page.m_square = -1;

  page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 }; page.m_av[0].m_square = 7;
  page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 }; page.m_av[1].m_square = 7;
  page.m_av[2].m_pattern = pattern[2];  page.m_av[2].m_board = { 5, 5 }; page.m_av[2].m_square = 7;
  page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 }; page.m_av[3].m_square = 7;

  page.m_legend = 70-12;

  page.m_table.clear();
  int total = page.m_board[0]*page.m_board[1];

  page.m_table.push_back( "#343434" );
   page.m_table.push_back( "#0885a1" );
   page.m_table.push_back( "#e0a32e" );
   page.m_table.push_back( "#67bdaa" );


  page.m_table.push_back( "#555555" );
   page.m_table.push_back( "#bb5695" );
   page.m_table.push_back( "#9dbc40" );
   page.m_table.push_back( "#8580b1" );

  page.m_table.push_back( "#7a7a79" );
   page.m_table.push_back( "#e7c71f" );
   page.m_table.push_back( "#5e3c6c" );
   page.m_table.push_back( "#576c43" );

  page.m_table.push_back( "#a0a0a0" );
   page.m_table.push_back( "#af363c" );
   page.m_table.push_back( "#c15a63" );
   page.m_table.push_back( "#627a9d" );

  page.m_table.push_back( "#c8c8c8" );
   page.m_table.push_back( "#469449" );
   page.m_table.push_back( "#505ba6" );
   page.m_table.push_back( "#c29682" );

  page.m_table.push_back( "#f3f3f2" );
   page.m_table.push_back( "#383d96" );
   page.m_table.push_back( "#d67e2c" );
   page.m_table.push_back( "#735244" );



  page.print( std::ofstream( fileName ) );
 }

#if defined( color_a9f1aad2_e23c_4f7e_9dce_0f1c116f4968 )

template < typename N_category >
void parallelogram
 (
   std::string                   const& fileName
  ,coord2D_t                     const& step
  ,coord3D_t                     const& O
  ,coord3D_t                     const& X
  ,coord3D_t                     const& Y
  ,std::array<std::uint64_t, 4 > const& pattern
 )
 {
  Page page;
  page.m_canvas = { 210, 297 };
  page.m_margin={ 20, 20 };
  page.m_square = -1;

  page.m_av[0].m_pattern = pattern[0];  page.m_av[0].m_board = { 5, 5 }; page.m_av[0].m_square = 7;
  page.m_av[1].m_pattern = pattern[1];  page.m_av[1].m_board = { 5, 5 }; page.m_av[1].m_square = 7;
  page.m_av[2].m_pattern = pattern[2];  page.m_av[2].m_board = { 5, 5 }; page.m_av[2].m_square = 7;
  page.m_av[3].m_pattern = pattern[3];  page.m_av[3].m_board = { 5, 5 }; page.m_av[3].m_square = 7;

  page.m_legend = 70-12;

  page.m_table.clear();

  typedef ::color::model< N_category > model_type;
  typedef ::color::rgb< double >         rgb_type;
  rgb_type rgb;

  for( double v=0; v < 1 ; v += step[1] )
   for( double u=0; u < 1 ; u += step[0] )
    {
     model_type point;
     point[0] = O[0] + u*X[0] + v* Y[0];
     point[1] = O[1] + u*X[1] + v* Y[1];
     point[2] = O[2] + u*X[2] + v* Y[2];
     rgb = point;
     if( true == color::check::overburn( rgb ) )
      {
       continue;
      }

     page.m_table.push_back( to_text( color8_type{ (std::uint8_t) ( rgb[0]*255),  (std::uint8_t)( rgb[1]*255 ), (std::uint8_t) ( rgb[2]*255 ) } ) );
    }

    {
     int space_w = ( int )( page.m_canvas[0] - 2* page.m_margin[0] - 2 * 7 * page.m_av[0].m_square );
     int space_h = ( int )( page.m_canvas[1] - 2* page.m_margin[1] - 2 * 7 * page.m_av[0].m_square );

     double unit = sqrt(  page.m_table.size() * ( space_w / (double)space_h ) );
     int h = int( unit + 0.5 );
     int w = int( unit * ( space_h / (double)space_w ) + 0.5 );
     if( h*w < page.m_table.size() ) w++;
     page.m_board = { (std::uint16_t)h, (std::uint16_t)w };
    }

  page.print( std::ofstream( fileName ) );
 }

#endif

void main()
 {
  BW(                                                                                                  { 0b0000100011100001110100111,0b0000100011011111001001110,0b0000100011011111000001111,0b0000100011011001110100111} );
  gray04(                                                                                              { 0b0000101011101111011001011,0b0000101011101101000110110,0b0000101011100111011001011,0b0000101011100101000110110} );
  gray08(                                                                                              { 0b0000101011101101000110110,0b0000101011100111011001011,0b0000101011100101000110110,0b0000101011011111001001111} );
  gray16(                                                                                              { 0b0000101011100111011001011,0b0000101011100101000110110,0b0000101011011111001001111,0b0000101011011011010110010} );
  gray32(                                                                                              { 0b0000101011100101000110110,0b0000101011011111001001111,0b0000101011011011010110010,0b0000101011010111001001111} );
  EIGHT(                                                                                               { 0b0000101011011111001001111,0b0000101011011011010110010,0b0000101011010111001001111,0b0000101011010011010110010} );
  cube27(                                                                                              { 0b0000101011011011010110010,0b0000101011010111001001111,0b0000101011010011010110010,0b0000101001111101110110110} );
  cube64(                                                                                              { 0b0000101011010111001001111,0b0000101011010011010110010,0b0000101001111101110110110,0b0000101001111101011100111} );
  red32L(                                                                                              { 0b0000101011010011010110010,0b0000101001111101110110110,0b0000101001111101011100111,0b0000101001111101000001111} );
  scale64( "A3-red64.svg",      { 255,   0,   0 },                                                     { 0b0000101001111101110110110,0b0000101001111101011100111,0b0000101001111101000001111,0b0000101001111011110110110} );
  scale64( "A3-green64.svg",    {   0, 255,   0 },                                                     { 0b0000101001111101011100111,0b0000101001111101000001111,0b0000101001111011110110110,0b0000101001111011110011011} );
  scale64( "A3-blue64.svg",     {   0,   0, 255 },                                                     { 0b0000101001111101000001111,0b0000101001111011110110110,0b0000101001111011110011011,0b0000101001111011010001011} );
  scale64( "A3-cyan64.svg",     {   0, 255, 255 },                                                     { 0b0000101001111011110110110,0b0000101001111011110011011,0b0000101001111011010001011,0b0000101001110101110110110} );
  scale64( "A3-magenta64.svg",  { 255,   0, 255 },                                                     { 0b0000101001111011110011011,0b0000101001111011010001011,0b0000101001110101110110110,0b0000101001110101011100111} );
  scale64( "A3-yellow64.svg",   { 255, 255,   0 },                                                     { 0b0000101001111011010001011,0b0000101001110101110110110,0b0000101001110101011100111,0b0000101001110101000001111} );
  scale32D( "A4-redD.svg",      { 255,   0,   0 },                                                     { 0b0000101001110101110110110,0b0000101001110101011100111,0b0000101001110101000001111,0b0000101001110011110110110} );
  scale32D( "A4-greenD.svg",    {   0, 255,   0 },                                                     { 0b0000101001110101011100111,0b0000101001110101000001111,0b0000101001110011110110110,0b0000101001110011110011011} );
  scale32D( "A4-blueD.svg",     {   0,   0, 255 },                                                     { 0b0000101001110101000001111,0b0000101001110011110110110,0b0000101001110011110011011,0b0000101001110011010001011} );
  scale32D( "A4-cyanD.svg",     {   0, 255, 255 },                                                     { 0b0000101001110011110110110,0b0000101001110011110011011,0b0000101001110011010001011,0b0000101001101111111001011} );
  scale32D( "A4-magentaD.svg",  { 255,   0, 255 },                                                     { 0b0000101001110011110011011,0b0000101001110011010001011,0b0000101001101111111001011,0b0000101001101111110001010} );
  scale32D( "A4-yellowD.svg",   { 255, 255,   0 },                                                     { 0b0000101001110011010001011,0b0000101001101111111001011,0b0000101001101111110001010,0b0000101001101001110110110} );
  lineA3( "A3-gray002.svg", { 0, 0, 0}, { 256.0/( 1* 2 ), 256.0/( 1* 2 ), 256.0/( 1* 2 ) }, { 1,  3 }, { 0b0000101001101111111001011,0b0000101001101111110001010,0b0000101001101001110110110,0b0000101001101001011100111} );
  lineA3( "A3-gray008.svg", { 0, 0, 0}, { 256.0/( 2* 4 ), 256.0/( 2* 4 ), 256.0/( 2* 4 ) }, { 2,  5 }, { 0b0000101001101111110001010,0b0000101001101001110110110,0b0000101001101001011100111,0b0000101001100111111001011} );
  lineA3( "A3-gray032.svg", { 0, 0, 0}, { 256.0/( 4* 8 ), 256.0/( 4* 8 ), 256.0/( 4* 8 ) }, { 4,  9 }, { 0b0000101001101001110110110,0b0000101001101001011100111,0b0000101001100111111001011,0b0000101001100111110001010} );
  lineA3( "A3-gray128b.svg",{ 0, 0, 0}, { 256.0/( 8*16 ), 256.0/( 8*16 ), 256.0/( 8*16 ) }, { 8, 16 }, { 0b0000101001101001011100111,0b0000101001100111111001011,0b0000101001100111110001010,0b0000101001100001110110110} );
  lineA3( "A3-gray006.svg", { 0, 0, 0}, { 256.0/( 2* 3 ), 256.0/( 2* 3 ), 256.0/( 2* 3 ) }, { 2,  4 }, { 0b0000101001100111111001011,0b0000101001100111110001010,0b0000101001100001110110110,0b0000101001100001011100111} );
  lineA3( "A3-gray024.svg", { 0, 0, 0}, { 256.0/( 4* 6 ), 256.0/( 4* 6 ), 256.0/( 4* 6 ) }, { 4,  7 }, { 0b0000101001100111110001010,0b0000101001100001110110110,0b0000101001100001011100111,0b0000101001011111101001111} );
  lineA3( "A3-gray096.svg", { 0, 0, 0}, { 256.0/( 8*12 ), 256.0/( 8*12 ), 256.0/( 8*12 ) }, { 8, 13 }, { 0b0000101001100001110110110,0b0000101001100001011100111,0b0000101001011111101001111,0b0000101001011111100001110} );
  cubeG( "A3-cube1.svg", {255/1.0,255/1.0,255/1.0},                                                    { 0b0000101001100001011100111,0b0000101001011111101001111,0b0000101001011111100001110,0b0000101001011001110110110} );
  cubeG( "A3-cube2.svg", {255/2.0,255/2.0,255/2.0},                                                    { 0b0000101001011111101001111,0b0000101001011111100001110,0b0000101001011001110110110,0b0000101001011001011100111} );
  cubeG( "A3-cube3.svg", {255/3.0,255/3.0,255/3.0},                                                    { 0b0000101001011111100001110,0b0000101001011001110110110,0b0000101001011001011100111,0b0000101001010111101001111} );
  cubeG( "A3-cube4.svg", {255/4.0,255/4.0,255/4.0},                                                    { 0b0000101001011001110110110,0b0000101001011001011100111,0b0000101001010111101001111,0b0000101001010111100001110} );
  cubeG( "A3-cube5.svg", {255/5.0,255/5.0,255/5.0},                                                    { 0b0000101001011001011100111,0b0000101001010111101001111,0b0000101001010111100001110,0b0000101001010001110110110} );
  cubeG( "A3-cube6.svg", {255/6.0,255/6.0,255/6.0},                                                    { 0b0000101001010111101001111,0b0000101001010111100001110,0b0000101001010001110110110,0b0000101001010001011100111} );
  lineA3( "A3-gray256.svg", { 0, 0, 0}, { 1, 1, 1 }, { 12,22 },                                        { 0b0000101001010111100001110,0b0000101001010001110110110,0b0000101001010001011100111,0b0000100111111101100100111 } );
  lineA3( "A3-gray004.svg", { 0, 0, 0}, { 256.0/( 2* 2 ), 256.0/( 2* 2 ), 256.0/( 2* 2 ) }, { 2,  3 }, { 0b0000101001010111100001110,0b0000101001010001110110110,0b0000101001010001011100111,0b0000100111111101100100111} );
  lineA3( "A3-gray016.svg", { 0, 0, 0}, { 256.0/( 4* 4 ), 256.0/( 4* 4 ), 256.0/( 4* 4 ) }, { 3,  6 }, { 0b0000101001010001110110110,0b0000101001010001011100111,0b0000100111111101100100111,0b0000100111111101001110110} );
  lineA3( "A3-gray064.svg", { 0, 0, 0}, { 256.0/( 8*8 ),  256.0/( 8*8 ),  256.0/( 8* 8 ) }, { 6, 11 }, { 0b0000101001010001011100111,0b0000100111111101100100111,0b0000100111111101001110110,0b0000100111111011001001010} );
  lineA3( "A3-gray128a.svg",{ 0, 0, 0}, { 256.0/( 8*16 ), 256.0/( 8*16 ), 256.0/( 8*16 ) }, { 9, 15 }, { 0b0000100111111101100100111,0b0000100111111101001110110,0b0000100111111011001001010,0b0000100111111011000001011} );

  lineA3( "A3-gray001.svg", { 0, 0, 0}, { 256.0/( 1* 1 ), 256.0/( 1* 1 ), 256.0/( 1* 1 ) }, { 1,  2 }, { 0b0000100111111101001110110,0b0000100111111011001001010,0b0000100111111011000001011,0b0000100111110101100100111} );
  
  lineA3( "A3-gray256b.svg", { 0, 0, 0}, { 1, 1, 1 }, { 16,16 }, { 0xffffffff,0xffffffff,0xffffffff,0xffffffff } );
  cubeG( "A3-cube008.svg", {255/8.0,255/8.0,255/8.0},            { 0xffffffff,0xffffffff,0xffffffff,0xffffffff } );
  cubeG( "A3-cube016.svg", {255/16.0,255/16.0,255/16.0},         { 0xffffffff,0xffffffff,0xffffffff,0xffffffff } );
  redALL( {255/7.0,255/7.0,255/7.0},                             { 0xffffffff,0xffffffff,0xffffffff,0xffffffff } );
  MacbethA4( "A4-Macbeth-Wikipedia.svg",                         { 0xffffffff,0xffffffff,0xffffffff,0xffffffff  } );

  planeA4<0,1>( "A4-plane-blue-0.svg", { 255.0/( 6 ), 255.0/( 6 ), 255.0/( 6 ) }, { 5, 10 }, { 0b0000111110001111010111100,0b0000110110111010000101011,0b0000100001101101101110110,0b0000100001100111110001011 } );

#if defined( color_a9f1aad2_e23c_4f7e_9dce_0f1c116f4968 )
parallelogram< color::lab<double>::category_type >
 (
   "lab-square.svg"
  ,{ 1/20.0, 1/20.0 }, {::color::lab<double>( ::color::constant::red_t{} )[0], -127, -127 }, { 0, 260, 0 }, { 0, 0, 260 }
  , { 0b0000111110001111010111100,0b0000110110111010000101011,0b0000100001101101101110110,0b0000100001100111110001011 } );

#endif

 }

/*
0b0000100011100001110100111,0b0000100011011111001001110,0b0000100011011111000001111,0b0000100011011001110100111
0b0000101011101111011001011,0b0000101011101101000110110,0b0000101011100111011001011,0b0000101011100101000110110
0b0000101011101101000110110,0b0000101011100111011001011,0b0000101011100101000110110,0b0000101011011111001001111
0b0000101011100111011001011,0b0000101011100101000110110,0b0000101011011111001001111,0b0000101011011011010110010
0b0000101011100101000110110,0b0000101011011111001001111,0b0000101011011011010110010,0b0000101011010111001001111
0b0000101011011111001001111,0b0000101011011011010110010,0b0000101011010111001001111,0b0000101011010011010110010
0b0000101011011011010110010,0b0000101011010111001001111,0b0000101011010011010110010,0b0000101001111101110110110
0b0000101011010111001001111,0b0000101011010011010110010,0b0000101001111101110110110,0b0000101001111101011100111
0b0000101011010011010110010,0b0000101001111101110110110,0b0000101001111101011100111,0b0000101001111101000001111
0b0000101001111101110110110,0b0000101001111101011100111,0b0000101001111101000001111,0b0000101001111011110110110
0b0000101001111101011100111,0b0000101001111101000001111,0b0000101001111011110110110,0b0000101001111011110011011
0b0000101001111101000001111,0b0000101001111011110110110,0b0000101001111011110011011,0b0000101001111011010001011
0b0000101001111011110110110,0b0000101001111011110011011,0b0000101001111011010001011,0b0000101001110101110110110
0b0000101001111011110011011,0b0000101001111011010001011,0b0000101001110101110110110,0b0000101001110101011100111
0b0000101001111011010001011,0b0000101001110101110110110,0b0000101001110101011100111,0b0000101001110101000001111
0b0000101001110101110110110,0b0000101001110101011100111,0b0000101001110101000001111,0b0000101001110011110110110
0b0000101001110101011100111,0b0000101001110101000001111,0b0000101001110011110110110,0b0000101001110011110011011
0b0000101001110101000001111,0b0000101001110011110110110,0b0000101001110011110011011,0b0000101001110011010001011
0b0000101001110011110110110,0b0000101001110011110011011,0b0000101001110011010001011,0b0000101001101111111001011
0b0000101001110011110011011,0b0000101001110011010001011,0b0000101001101111111001011,0b0000101001101111110001010
0b0000101001110011010001011,0b0000101001101111111001011,0b0000101001101111110001010,0b0000101001101001110110110
0b0000101001101111111001011,0b0000101001101111110001010,0b0000101001101001110110110,0b0000101001101001011100111
0b0000101001101111110001010,0b0000101001101001110110110,0b0000101001101001011100111,0b0000101001100111111001011
0b0000101001101001110110110,0b0000101001101001011100111,0b0000101001100111111001011,0b0000101001100111110001010
0b0000101001101001011100111,0b0000101001100111111001011,0b0000101001100111110001010,0b0000101001100001110110110
0b0000101001100111111001011,0b0000101001100111110001010,0b0000101001100001110110110,0b0000101001100001011100111
0b0000101001100111110001010,0b0000101001100001110110110,0b0000101001100001011100111,0b0000101001011111101001111
0b0000101001100001110110110,0b0000101001100001011100111,0b0000101001011111101001111,0b0000101001011111100001110
0b0000101001100001011100111,0b0000101001011111101001111,0b0000101001011111100001110,0b0000101001011001110110110
0b0000101001011111101001111,0b0000101001011111100001110,0b0000101001011001110110110,0b0000101001011001011100111
0b0000101001011111100001110,0b0000101001011001110110110,0b0000101001011001011100111,0b0000101001010111101001111
0b0000101001011001110110110,0b0000101001011001011100111,0b0000101001010111101001111,0b0000101001010111100001110
0b0000101001011001011100111,0b0000101001010111101001111,0b0000101001010111100001110,0b0000101001010001110110110
0b0000101001010111101001111,0b0000101001010111100001110,0b0000101001010001110110110,0b0000101001010001011100111
0b0000101001010111100001110,0b0000101001010001110110110,0b0000101001010001011100111,0b0000100111111101100100111
0b0000101001010001110110110,0b0000101001010001011100111,0b0000100111111101100100111,0b0000100111111101001110110
0b0000101001010001011100111,0b0000100111111101100100111,0b0000100111111101001110110,0b0000100111111011001001010
0b0000100111111101100100111,0b0000100111111101001110110,0b0000100111111011001001010,0b0000100111111011000001011
0b0000100111111101001110110,0b0000100111111011001001010,0b0000100111111011000001011,0b0000100111110101100100111
0b0000100111111011001001010,0b0000100111111011000001011,0b0000100111110101100100111,0b0000100111110101001110110
0b0000100111111011000001011,0b0000100111110101100100111,0b0000100111110101001110110,0b0000100111110011001001010
0b0000100111110101100100111,0b0000100111110101001110110,0b0000100111110011001001010,0b0000100111110011000001011
0b0000100111110101001110110,0b0000100111110011001001010,0b0000100111110011000001011,0b0000100111101111101001011
0b0000100111110011001001010,0b0000100111110011000001011,0b0000100111101111101001011,0b0000100111101111001011011
0b0000100111110011000001011,0b0000100111101111101001011,0b0000100111101111001011011,0b0000100111101001111001111
0b0000100111101111101001011,0b0000100111101111001011011,0b0000100111101001111001111,0b0000100111101001100100111
0b0000100111101111001011011,0b0000100111101001111001111,0b0000100111101001100100111,0b0000100111100111101001011
0b0000100111101001111001111,0b0000100111101001100100111,0b0000100111100111101001011,0b0000100111100111001011011
0b0000100111101001100100111,0b0000100111100111101001011,0b0000100111100111001011011,0b0000100111100001111001111
0b0000100111100111101001011,0b0000100111100111001011011,0b0000100111100001111001111,0b0000100111100001100100111
0b0000100111100111001011011,0b0000100111100001111001111,0b0000100111100001100100111,0b0000100111001101100100111
0b0000100111100001111001111,0b0000100111100001100100111,0b0000100111001101100100111,0b0000100111001011011001110
0b0000100111100001100100111,0b0000100111001101100100111,0b0000100111001011011001110,0b0000100111001011010001111
0b0000100111001101100100111,0b0000100111001011011001110,0b0000100111001011010001111,0b0000100111000101100100111
0b0000100111001011011001110,0b0000100111001011010001111,0b0000100111000101100100111,0b0000100111000011011001110
0b0000100111001011010001111,0b0000100111000101100100111,0b0000100111000011011001110,0b0000100111000011010001111
0b0000100111000101100100111,0b0000100111000011011001110,0b0000100111000011010001111,0b0000100101111011100001011
0b0000100111000011011001110,0b0000100111000011010001111,0b0000100101111011100001011,0b0000100101110011100001011
0b0000100111000011010001111,0b0000100101111011100001011,0b0000100101110011100001011,0b0000100101101111001001011
0b0000100101111011100001011,0b0000100101110011100001011,0b0000100101101111001001011,0b0000100101101101101110010
0b0000100101110011100001011,0b0000100101101111001001011,0b0000100101101101101110010,0b0000100101101001011001111
0b0000100101101111001001011,0b0000100101101101101110010,0b0000100101101001011001111,0b0000100101100111001001011
0b0000100101101101101110010,0b0000100101101001011001111,0b0000100101100111001001011,0b0000100101100101101110010
0b0000100101101001011001111,0b0000100101100111001001011,0b0000100101100101101110010,0b0000100101100001011001111
0b0000100101100111001001011,0b0000100101100101101110010,0b0000100101100001011001111,0b0000100101001111101110010
0b0000100101100101101110010,0b0000100101100001011001111,0b0000100101001111101110010,0b0000100101001011110001111
0b0000100101100001011001111,0b0000100101001111101110010,0b0000100101001011110001111,0b0000100101000111101110010
0b0000100101001111101110010,0b0000100101001011110001111,0b0000100101000111101110010,0b0000100101000011110001111
0b0000100101001011110001111,0b0000100101000111101110010,0b0000100101000011110001111,0b0000100011111101110100111
0b0000100101000111101110010,0b0000100101000011110001111,0b0000100011111101110100111,0b0000100011111101101001111
0b0000100101000011110001111,0b0000100011111101110100111,0b0000100011111101101001111,0b0000100011111101011110110
0b0000100011111101110100111,0b0000100011111101101001111,0b0000100011111101011110110,0b0000100011111011111001011
0b0000100011111101101001111,0b0000100011111101011110110,0b0000100011111011111001011,0b0000100011111011011110110
0b0000100011111101011110110,0b0000100011111011111001011,0b0000100011111011011110110,0b0000100011111011011011011
0b0000100011111011111001011,0b0000100011111011011110110,0b0000100011111011011011011,0b0000100011110101110100111
0b0000100011111011011110110,0b0000100011111011011011011,0b0000100011110101110100111,0b0000100011110101101001111
0b0000100011111011011011011,0b0000100011110101110100111,0b0000100011110101101001111,0b0000100011110101011110110
0b0000100011110101110100111,0b0000100011110101101001111,0b0000100011110101011110110,0b0000100011110011111001011
0b0000100011110101101001111,0b0000100011110101011110110,0b0000100011110011111001011,0b0000100011110011011110110
0b0000100011110101011110110,0b0000100011110011111001011,0b0000100011110011011110110,0b0000100011110011011011011
0b0000100011110011111001011,0b0000100011110011011110110,0b0000100011110011011011011,0b0000100011101111011001010
0b0000100011110011011110110,0b0000100011110011011011011,0b0000100011101111011001010,0b0000100011101111010001011
0b0000100011110011011011011,0b0000100011101111011001010,0b0000100011101111010001011,0b0000100011101001110100111
0b0000100011101111011001010,0b0000100011101111010001011,0b0000100011101001110100111,0b0000100011100111011001010
0b0000100011101111010001011,0b0000100011101001110100111,0b0000100011100111011001010,0b0000100011100111010001011
0b0000100011101001110100111,0b0000100011100111011001010,0b0000100011100111010001011,0b0000100011100001110100111
0b0000100011100111011001010,0b0000100011100111010001011,0b0000100011100001110100111,0b0000100011011111001001110
0b0000100011100111010001011,0b0000100011100001110100111,0b0000100011011111001001110,0b0000100011011111000001111
0b0000100011011111001001110,0b0000100011011111000001111,0b0000100011011001110100111,0b0000100011011001011110110
0b0000100011011111000001111,0b0000100011011001110100111,0b0000100011011001011110110,0b0000100011010111001001110
0b0000100011011001110100111,0b0000100011011001011110110,0b0000100011010111001001110,0b0000100011010111000001111
0b0000100011011001011110110,0b0000100011010111001001110,0b0000100011010111000001111,0b0000100011010001110100111
0b0000100011010111001001110,0b0000100011010111000001111,0b0000100011010001110100111,0b0000100011010001011110110
0b0000100011010111000001111,0b0000100011010001110100111,0b0000100011010001011110110,0b0000100001111101001001111
0b0000100011010001110100111,0b0000100011010001011110110,0b0000100001111101001001111,0b0000100001111011011001011
0b0000100011010001011110110,0b0000100001111101001001111,0b0000100001111011011001011,0b0000100001111001101110110
0b0000100001111101001001111,0b0000100001111011011001011,0b0000100001111001101110110,0b0000100001110101001001111
0b0000100001111011011001011,0b0000100001111001101110110,0b0000100001110101001001111,0b0000100001110011011001011
0b0000100001111001101110110,0b0000100001110101001001111,0b0000100001110011011001011,0b0000100001110001101110110
0b0000100001110101001001111,0b0000100001110011011001011,0b0000100001110001101110110,0b0000100001101111110001011
0b0000100001110011011001011,0b0000100001110001101110110,0b0000100001101111110001011,0b0000100001101101101110110
0b0000111110001111010111100,0b0000110110111010000101011,0b0000100001101101101110110,0b0000100001100111110001011
*/