//////////////////////////////////////////////////////////////////////////////
// Program Name: guide.h
// Created     : Mar. 7, 2011
//
// Copyright (c) 2011 David Blain <dblain@mythtv.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef GUIDE_H
#define GUIDE_H

#include <QScriptEngine>

#include "serviceUtil.h"

#include "services/guideServices.h"

#include "datacontracts/programAndChannel.h"
#include "datacontracts/channelGroupList.h"
#include "datacontracts/programList.h"
#include "programinfo.h"

class Guide : public GuideServices
{
    Q_OBJECT

    public:
    
        Q_INVOKABLE Guide( QObject *parent = 0 ) {}

    public:

        
        DTC::ProgramGuide*  GetProgramGuide     ( const QDateTime &StartTime  ,
                                                  const QDateTime &EndTime    ,
                                                  int              StartChanId,
                                                  int              NumChannels,
                                                  bool             Details,
                                                  int              ChannelGroupId );

        DTC::ProgramList*   GetProgramList      ( int              StartIndex,
                                                  int              Count,
                                                  const QDateTime &StartTime  ,
                                                  const QDateTime &EndTime    ,
                                                  int              ChanId,
                                                  const QString   &TitleFilter,
                                                  const QString   &CategoryFilter,
                                                  const QString   &PersonFilter,
                                                  const QString   &KeywordFilter,
                                                  bool             OnlyNew,
                                                  bool             Details,
                                                  const QString   &Sort,
                                                  bool             Descending );

        DTC::Program*       GetProgramDetails   ( int              ChanId,
                                                  const QDateTime &StartTime );

        QFileInfo           GetChannelIcon      ( int              ChanId,
                                                  int              Width ,
                                                  int              Height );

        DTC::ChannelGroupList*  GetChannelGroupList ( bool         IncludeEmpty );

        QStringList         GetCategoryList     ( );

        QStringList         GetStoredSearches( const QString   &Type );
};

// --------------------------------------------------------------------------
// The following class wrapper is due to a limitation in Qt Script Engine.  It
// requires all methods that return pointers to user classes that are derived from
// QObject actually return QObject* (not the user class *).  If the user class pointer
// is returned, the script engine treats it as a QVariant and doesn't create a
// javascript prototype wrapper for it.
// 
// This class allows us to keep the rich return types in the main API class while
// offering the script engine a class it can work with.
// 
// Only API Classes that return custom classes needs to implement these wrappers.
//
// We should continue to look for a cleaning solution to this problem.
// --------------------------------------------------------------------------

class ScriptableGuide : public QObject
{
    Q_OBJECT

    private:

        Guide    m_obj;

    public:
    
        Q_INVOKABLE ScriptableGuide( QObject *parent = 0 ) : QObject( parent ) {}

    public slots:

        QObject* GetProgramGuide( const QDateTime &StartTime  ,
                                  const QDateTime &EndTime    ,
                                  int              StartChanId,
                                  int              NumChannels,
                                  bool             Details,
                                  int              ChannelGroupId )
        {
            return m_obj.GetProgramGuide( StartTime, EndTime, StartChanId, NumChannels, Details, ChannelGroupId );
        }

        QObject* GetProgramList(int              StartIndex,
                                int              Count,
                                const QDateTime &StartTime,
                                const QDateTime &EndTime,
                                int              ChanId,
                                const QString   &TitleFilter,
                                const QString   &CategoryFilter,
                                const QString   &PersonFilter,
                                const QString   &KeywordFilter,
                                bool             OnlyNew,
                                bool             Details,
                                const QString   &Sort,
                                bool             Descending)
        {
            return m_obj.GetProgramList( StartIndex, Count,
                                         StartTime, EndTime, ChanId,
                                         TitleFilter, CategoryFilter,
                                         PersonFilter, KeywordFilter,
                                         OnlyNew, Details,
                                         Sort, Descending );
        }

        QObject* GetProgramDetails( int ChanId, const QDateTime &StartTime )
        {
            return m_obj.GetProgramDetails( ChanId, StartTime );
        }

        QFileInfo GetChannelIcon( int ChanId, int Width, int Height )
        {
            return m_obj.GetChannelIcon( ChanId, Width, Height );
        }

        QObject* GetChannelGroupList( bool IncludeEmpty = false )
        {
            return m_obj.GetChannelGroupList( IncludeEmpty );
        }

        QStringList GetCategoryList( )
        {
            return m_obj.GetCategoryList();
        }

        QStringList GetStoredSearches( QString Type )
        {
            return m_obj.GetStoredSearches( Type );
        }
};

Q_SCRIPT_DECLARE_QMETAOBJECT( ScriptableGuide, QObject*);

#endif 
