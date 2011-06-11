#!/usr/bin/env python
# -*- coding: utf-8 -*-
#

#
# This file is licensed under BSD license.
#
# Copyright (c) 2011, Vincit Oy -- http://www.vincit.fi/
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# * Neither the name of the Vincit Oy nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#


#
# Usage: lgen <command> <command_arguments>
#
# Available commands are parse, combine and merge.
# - Parse parser C++ files and gathers the translatable strings into a XML file.
# - Combine combines data from files generated in parse step into a single XML file
# - Merge merges new items from the single XML file generated in the combine step
#   into another XML file and does not change possibly translated items.
#
# Workflow
# --------
#
# Makefile example
# ----------------
# ===
# TR_FILES:=translations/module/a.cpp translations/module/a.hpp translations/main.cpp
#
# tmp/translations/%.xml: src/%
#   lgen parse ${<} -o ${@}
#
# tmp/translations/combined.xml: ${TR_FILES}
#   lgen combine -o ${@} ${TR_FILES}
#
# translations/%.xml: tmp/translations/combined.xml
#   lgen merge ${<} ${@} -o ${@}
#
# TRANSLATIONS:=english finnish
# translations: ${TRANSLATIONS}
# ===
#
# The above makefile uses lgen to parse translations from three files (a.cpp, a.hpp and main.cpp).
#
# The first rule parser translations from C++ files into XML files containing strings from one file.
# For example, strings from src/main.cpp is put into tmp/translations/main.cpp.xml
#
# The second rule combines all the XML files into one big XML file containing all the strings.
#
# The third rule merges strings form combined.xml into translatable files. This step does
# not remove changes made in the target XML file unless the string is removed from the code.
#
#
# What are to be parsed:
# TR_FN_NAMES is a list containing function names that are searched from the C++ code and the
# string given as parameter is read as translation string ID.
#


import optparse
import os
import os.path
import re
import sys
import xml.etree.ElementTree

TR_FN_NAMES = [ u'tr' ]

def xmlEscape( s ):
  return s.replace( u'&', u'&amp;' ).replace( u'<', u'&lt;' ).replace( u'>', u'&gt;' ).replace( u'"', u'&quot;' )

class TrReader( object ):
  def __init__( self, file ):
    object.__init__( self )
    self.ids = set()
    class ByteStream( object ):
      def __init__( self, file ):
        object.__init__( self )
        s = u''
        for line in file:
          try:
            s += line.decode( 'utf-8' )
          except Exception:
            print 'Offending line: "%s"' % line
            raise
        self._iter = iter( s )
        self._nextByte = self._iter.next()

      def hasData( self ):
        return self._nextByte is not None

      def get( self ):
        byte = self._nextByte
        try:
          self._nextByte = self._iter.next()
        except StopIteration:
          self._nextByte = None
        return byte

    self._input = ByteStream( file )
    self._trFnRes = []
    for trFnName in TR_FN_NAMES:
      trFnNameRe = re.compile( u'^' + trFnName + u'[ \\t\\n\\r]*\\(' )
      self._trFnRes.append( ( trFnName, trFnNameRe ) )
    self._whitespaceRe = re.compile( r'^[ \t\n\r]+$' )
                        

  def _moreDataNeededForTrFn( self, s ):
    result = False

    for trFnName, trFnNameRe in self._trFnRes:
      if len( s ) > trFnName:
        part = s[ 0 : len( trFnName ) ]
        if s == trFnName and self._whitespaceRe.match( s[ len( trFnName ) ] ):
          result = True
      elif trFnName.startswith( s ):
        result = True

    return result

  def _isTrFn( self, s ):
    result = False

    for trFnName, trFnNameRe in self._trFnRes:
      if trFnNameRe.match( s ):
        result = True

    return result

  def __call__( self ):
    ids = set()

    while self._input.hasData():
      s = self._input.get()
      #while TRANSLATED_STRING_FN.startswith( s ) and len( s ) < len( TRANSLATED_STRING_FN ):
      while self._moreDataNeededForTrFn( s ):
        s += self._input.get()

      if self._isTrFn( s ):
        trstring = self.readPossibleTrString()
        if trstring is not None:
          ids.add( trstring )

    return ids
  
  def skipWhiteSpace( self ):
    byte = self._input.get()
    while byte in u' \t\n\r':
      byte = self._input.get()
    return byte

  def readPossibleTrString( self ):
    return self.readString()

  def readString( self ):
    byte = self.skipWhiteSpace()
    if byte != u'"':
      return None

    res = u''
    byte = self._input.get()
    stringEnded = False
    while not stringEnded:
      while byte != u'"':
        res += byte
        byte = self._input.get()

      if byte == u'"' and\
         len( res ) > 0 and\
         res[ -1 ] == u'\\' and\
         ( len( res ) == 1 or res[ -2 ] != u'\\' ):
        res = res[ :-1 ] + u'"'
        byte = self._input.get()
      else:
        byte = self.skipWhiteSpace()
        if byte == u'"':
          byte = self._input.get()
        else:
        #elif byte == u')':
          stringEnded = True

    return res




class XmlWriter( object ):
  class IndFileObj( object ):
    def __init__( self, fileobj ):
      object.__init__( self )
      self.__fileobj = fileobj
      self._indent = ''
      self.__indentWidth = 2

    def indent( self ):
      self._indent += ' ' * self.__indentWidth

    def unindent( self ):
      self._indent = self._indent[ 0 : -1 * self.__indentWidth ]

    def __enter__( self ):
      self.__fileobj.__enter__()
      return self
    
    def writeline( self, line ):
      if line is None:
        line = ''
      else:
        line = line.encode( 'utf-8' )

      self.__fileobj.write( self._indent + line + '\n' )

    def __exit__( self, *args ):
      self.__fileobj.__exit__()

    def __getattr__( self, name ):
      return getattr( self.__fileobj, name )

  class Tag( object ):
    def __init__( self, out, name, **kwargs ):
      object.__init__( self )
      self.__out = out
      self.__name = name
      self.__attrs = kwargs
      self.data = None

    def __enter__( self ):
      startTag = u'<' + self.__name
      for attrName, attrValue in self.__attrs.items():
        startTag += u' ' + unicode( attrName ) + u'="' + unicode( attrValue ) + u'"'
      startTag += u'>'

      self.__out.writeline( startTag )
      self.__out.indent()

      return self
    
    def __exit__( self, *args ):
      if self.data is not None:
        self.__out.writeline( self.data )

      self.__out.unindent()
      endTag = u'</' + self.__name + u'>'
      self.__out.writeline( endTag )


  def __init__( self, output ):
    object.__init__( self )
    self._output = self.IndFileObj( output )

  def __enter__( self ):
    self._output.__enter__()
    return self

  def __exit__( self, *args ):
    self._output.__exit__()

  def write( self, tree ):
    self.element( tree.getroot() )

  def element( self, elem ):
    if elem.text is None:
      with self.Tag( self._output,
                     name = elem.tag,
                     **elem.attrib ) as tag:
        for subElem in elem.getchildren():
          self.element( subElem )
    else:
      t = u'<' + unicode( elem.tag )
      for attrName, attrValue in elem.attrib.items():
        t += u' ' + unicode( attrName ) + u'="' + xmlEscape( unicode( attrValue ) ) + u'"'
      t += u'>' + xmlEscape( unicode( elem.text ) ) + u'</' + unicode( elem.tag ) + u'>'
      self._output.writeline( t )




class StreamWrapper( object ):
  def __init__( self, stream ):
    object.__init__( self )
    self.__stream = stream

  def __enter__( self ):
    return self
  
  def __exit__( self, *args ):
    pass

  def __getattr__( self, name ):
    return getattr( self.__stream, name )


class MetaCommand( type ):
  def __new__( mcs, name, bases, dict ):
    return type.__new__( mcs, name, bases, dict )

class Command( object ):
  def __init__( self, cmd, args ):
    object.__init__( self )
    self.__cmd = cmd
    self.__args = args
    self._openOutputFile = lambda: StreamWrapper( sys.stdout )

  def configure( self ):
    self.parseArgs()

    if self.opts.outputFile is not None:
      self._openOutputFile = lambda: open( self.opts.outputFile, 'w+' )

    

  def parseArgs( self ):
    op = optparse.OptionParser( prog = self.__cmd )
    op.add_option( '-o',
                   '',
                   dest = u'outputFile',
                   default = None,
                   help = u'Output file.' )

    self.configureOptParser( op )

    self.opts, self.args =  op.parse_args( self.__args )
    if not self.hasValidOptsAndArgs():
      op.print_help()
      sys.exit( 1 )

      

  def configureOptParser( self, optParser ):
    pass

  def hasValidOptsAndArgs( self ):
    return True

  def __call__( self ):
    self.configure()
    self.main()

  def write( self, tree ):
    with XmlWriter( self._openOutputFile() ) as o:
      o.write( tree )


class TrParser( Command ):
  def __init__( self, cmd, args ):
    Command.__init__( self, cmd, args )

  def main( self ):
    filename = self.args[0]
    trIds = set()

    trIds.update( self.readFile( filename ) )

    tree = self.createEtree( trIds )
    self.write( tree )

  def readFile( self, filename ):
    with open( filename, u'r' ) as f:
      ids = self.parseTrIds( f )
    return ids
    
  def parseTrIds( self, file ):
    reader = TrReader( file )
    ids = reader()
    return ids

  def createEtree( self, trIds ):
    trIds = list( trIds )
    trIds.sort()

    builder = xml.etree.ElementTree.TreeBuilder()
    builder.start( u'messages', attrs = {} )
    for trid in trIds:
      builder.start( u'message', attrs = { 'id': trid } )
      builder.data( trid )
      builder.end( u'message' )
    builder.end( u'messages' )
    root = builder.close()

    return xml.etree.ElementTree.ElementTree( root )
  

  def configureOptParser( self, optParser ):
    optParser.usage += u' <file>'

  def hasValidOptsAndArgs( self ):
    return len( self.args ) == 1


class TrFileReader( Command ):
  def readTrFile( self, filename ):
    elems = {}
    tree = xml.etree.ElementTree.parse( filename )
    for elem in tree.getiterator( u'message' ):
      if elem.text is not None:
        elem.text = elem.text.strip( u'\n\t\r ' )
      elems[ elem.attrib[ 'id' ] ] = elem

    return elems

  def createMessagesTree( self, elems ):
    rootElem = xml.etree.ElementTree.Element( u'messages' )
    keys = elems.keys()
    keys.sort()
    for key in keys:
      rootElem.append( elems[ key ] )

    return xml.etree.ElementTree.ElementTree( rootElem )

class Combiner( TrFileReader ):
  def configureOptParser( self, optParser ):
    optParser.usage += u' <files>'

  def hasValidOptsAndArgs( self ):
    return len( self.args ) > 0

  def main( self ):
    elems = self.readTrFiles( self.args )
    tree = self.createMessagesTree( elems )
    self.write( tree )

  def readTrFiles( self, filenames ):
    elems = {}
    for filename in filenames:
      tree = xml.etree.ElementTree.parse( filename )
      for elem in tree.getiterator( u'message' ):
        if elem.text is not None:
          elem.text = elem.text.strip( u'\n\t\r ' )
        else:
          elem.text = u''
        if elem.attrib[ 'id' ] not in elems:
          elems[ elem.attrib[ 'id' ] ] = elem

    return elems

class Merge( TrFileReader ):
  def configureOptParser( self, optParser ):
    optParser.usage += u' <from> <to>'

  def hasValidOptsAndArgs( self ):
    return len( self.args ) == 2

  def main( self ):
    srcElems = self.readTrFile( self.args[0] )
    if os.path.isfile( self.args[1] ):
      targetElems = self.readTrFile( self.args[1] )
    else:
      targetElems = {}

    srcIds = set( srcElems.keys() )
    targetIds = set( targetElems.keys() )

    # Remove ids that are not in the source any more, i.e.
    # the removed translatable strings
    removedIds = targetIds.difference( srcIds )

    for trId in removedIds:
      targetElems.pop( trId )

    elems = srcElems
    elems.update( targetElems )

    tree = self.createMessagesTree( elems )

    self.write( tree )

if __name__ == u'__main__':
  if len( sys.argv ) < 2:
    print u'No command given.'
    sys.exit( 1 )

  if sys.argv[1] == u'parse':
    cmdFactory = TrParser
  elif sys.argv[1] == u'combine':
    cmdFactory= Combiner
  elif sys.argv[1] == u'merge':
    cmdFactory= Merge
  else:
    print u'Invalid command'
    sys.exit( 1 )

  cmd = cmdFactory( sys.argv[0] + sys.argv[1], sys.argv[ 2: ] )
  cmd()

