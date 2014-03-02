/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import com.github.radekp 1.0

ApplicationWindow
{
    // Cover
    CoverBackground {
        id: myCover

        ChargeGraph {
            id: coverGraph
            width: parent.width
            height: parent.height
        }

        CoverActionList {
            id: coverAction

            CoverAction {
                iconSource: "image://theme/icon-cover-refresh"
                onTriggered: {
                    label.text = mon.readCharge()
                    graph.update()
                    coverGraph.update()
                }
            }
        }
    }

    // First page
    Page {

        id: firstPage
        allowedOrientations: Orientation.All

        // To enable PullDownMenu, place our content in a SilicaFlickable
        SilicaFlickable {
            anchors.fill: parent

            // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
            PullDownMenu {

                MenuItem {
                    id: timerToggle
                    text: "Disable auto refresh"
                    onClicked: {
                        if(timer.interval == 0) {
                            timer.interval = 3000
                            timerToggle.text = "Disable auto refresh"
                            readCharge.visible = false
                        }
                        else {
                            timer.interval = 0;
                            timerToggle.text = "Enable auto refresh"
                            readCharge.visible = true
                        }
                    }
                }
                MenuItem {
                    id: readCharge
                    text: "Read charge"
                    onClicked: {
                        label.text = mon.readCharge()
                        graph.update()
                        coverGraph.update()
                    }
                }
            }

            // Tell SilicaFlickable the height of its content.
            contentHeight: column.height

            // Place our content in a Column.  The PageHeader is always placed at the top
            // of the page, followed by our content.
            Column {
                id: column

                width: firstPage.width
                spacing: Theme.paddingLarge

                ChargeGraph {
                    id: graph
                    width: firstPage.width
                    height: firstPage.height
                }

                Label {
                    id: label
                    x: Theme.paddingLarge
                    text: "Reading battery status..."
                    font.pixelSize: Theme.fontSizeExtraSmall
                }
                Timer {
                    id: timer
                    interval: 1000; running: true; repeat: true
                    onTriggered: {
                        label.text = mon.readCharge()
                        timer.interval = 3000
                        graph.update()
                        coverGraph.update()
                    }
                }
            }
        }
    }

    // ApplicationWindow properties
    cover: myCover
    initialPage: firstPage

}


