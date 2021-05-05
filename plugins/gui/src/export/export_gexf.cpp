#include "gui/export/export_gexf.h"
#include <QFileDialog>
#include <QDir>
#include <QApplication>
#include <QFile>
#include <QDate>
#include <QColor>
#include "gui/gui_globals.h"
#include "gui/netlist_relay/netlist_relay.h"
#include "hal_version.h"
#include "hal_core/netlist/gate.h"
#include "hal_core/netlist/net.h"

namespace hal {
    ExportGexf::ExportGexf(const QString& filename)
        : mFilename(filename), mEdgeId(0)
    {;}

    bool ExportGexf::queryFilename()
    {
        QWidget* parent = qApp->activeWindow();
        mFilename = QFileDialog::getSaveFileName(
                    parent,
                    "Export netlist",
                    QDir::currentPath(),
                    "GEXF graph file (.gexf)",
                    nullptr);
        if (mFilename.isEmpty()) return false;

        if (!mFilename.contains('.')) mFilename.append(".gexf");
        return true;
    }

    bool ExportGexf::exportNetlist()
    {
        if (mFilename.isEmpty() || !gNetlist) return false;
        QFile of(mFilename);
        if (!of.open(QIODevice::WriteOnly)) return false;
        QXmlStreamWriter xmlOut(&of);
        xmlOut.setAutoFormatting(true);
        xmlOut.writeStartDocument();
        xmlOut.writeStartElement("gexf");
        xmlOut.writeAttribute("xmlns", "http://www.gexf.net/1.2draft");
 //       xmlOut.writeNamespace("http://www.gexf.net/1.2draft/viz");  // visualisation attributes
        xmlOut.writeNamespace("http://www.w3.org/2001/XMLSchema-instance", "xsi");
        xmlOut.writeAttribute("http://www.w3.org/2001/XMLSchema-instance","schemaLocation","http://www.gexf.net/1.2draft http://www.gexf.net/1.2draft/gexf.xsd");
        xmlOut.writeAttribute("version", "1.2");
        writeMeta(xmlOut);
        writeGraph(xmlOut);
        xmlOut.writeEndElement();
        xmlOut.writeEndDocument();
        return true;
    }

    void ExportGexf::writeMeta(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeStartElement("meta");
        xmlOut.writeAttribute("lastmodifieddate", QDate::currentDate().toString("yyyy-MM-dd"));
        xmlOut.writeTextElement("creator", QString("hal %1.%2.%3")
                                .arg(hal_version::major)
                                .arg(hal_version::minor)
                                .arg(hal_version::patch));
        xmlOut.writeEndElement();
    }

    void ExportGexf::writeGraph(QXmlStreamWriter& xmlOut)
    {
        xmlOut.writeStartElement("graph");
        xmlOut.writeAttribute("defaultedgetype", "directed");
        xmlOut.writeAttribute("mode", "static");
        // Edge attributes
        xmlOut.writeStartElement("attributes");
        xmlOut.writeAttribute("mode", "static");
        xmlOut.writeAttribute("class", "edge");
        writeAttribute(xmlOut, 3, "label", "string");
        writeAttribute(xmlOut, 4, "hal_id", "long");
        writeAttribute(xmlOut, 5, "source_pin", "string");
        writeAttribute(xmlOut, 6, "destination_pin", "string");
        writeAttribute(xmlOut, 7, "networkx_key", "long");
        xmlOut.writeEndElement();
        // Node attributes
        xmlOut.writeStartElement("attributes");
        xmlOut.writeAttribute("mode", "static");
        xmlOut.writeAttribute("class", "node");
        writeAttribute(xmlOut, 0, "type", "string");
        writeAttribute(xmlOut, 1, "module", "string");
        writeAttribute(xmlOut, 2, "INIT", "string");
        xmlOut.writeEndElement();
        // end attributes

        xmlOut.writeStartElement("nodes");
        for (const Gate* g : gNetlist->get_gates())
            writeNode(xmlOut, g);
        xmlOut.writeEndElement();

        xmlOut.writeStartElement("edges");
        for (const Net* n : gNetlist->get_nets())
            writeEdge(xmlOut, n);
        xmlOut.writeEndElement();

    }

    void ExportGexf::writeAttribute(QXmlStreamWriter& xmlOut, int id, const QString& title, const QString& type) const
    {
        xmlOut.writeStartElement("attribute");
        xmlOut.writeAttribute("id", QString::number(id));
        xmlOut.writeAttribute("title", title);
        xmlOut.writeAttribute("type", type);
        xmlOut.writeEndElement();
    }


    void ExportGexf::writeNode(QXmlStreamWriter& xmlOut, const Gate* g) const
    {
        xmlOut.writeStartElement("node");
        xmlOut.writeAttribute("id", QString::number(g->get_id()));
        xmlOut.writeAttribute("label", QString::fromStdString(g->get_name()));
        writeColor(xmlOut, g);
        xmlOut.writeStartElement("attvalues");
        writeNodeAttribute(xmlOut,g,0);
        writeNodeAttribute(xmlOut,g,1);
        writeNodeAttribute(xmlOut,g,2);
        xmlOut.writeEndElement();
        xmlOut.writeEndElement();
    }

    void ExportGexf::writeColor(QXmlStreamWriter &xmlOut, const Gate *g) const
    {
        QColor col = gNetlistRelay->getModuleColor(g->get_module()->get_id());
        if (!col.isValid()) return;
//        xmlOut.writeStartElement("http://www.gexf.net/1.2draft/viz","color");
        xmlOut.writeStartElement("color");
        xmlOut.writeAttribute("r", QString::number(col.red()));
        xmlOut.writeAttribute("g", QString::number(col.green()));
        xmlOut.writeAttribute("b", QString::number(col.blue()));
        xmlOut.writeEndElement();
    }

    void ExportGexf::writeNodeAttribute(QXmlStreamWriter& xmlOut, const Gate* g, int inx) const
    {
        QString value;
        switch(inx)
        {
        case 0:  value = QString::fromStdString(g->get_type()->get_name()); break;
        case 1:  value = QString::fromStdString(g->get_module()->get_name()); break;
        case 2:
            for(const auto& [key, val] : g->get_data_map())
            {
                QString keyTxt      = QString::fromStdString(std::get<1>(key));
                if (keyTxt == "INIT")
                {
                    value = QString::fromStdString(std::get<1>(val));
                    break;
                }
            }
            break;
        }

        if (value.isEmpty()) return;

        xmlOut.writeStartElement("attvalue");
        xmlOut.writeAttribute("for", QString::number(inx));
        xmlOut.writeAttribute("value", value);
        xmlOut.writeEndElement();
    }

    void ExportGexf::writeEdge(QXmlStreamWriter& xmlOut, const Net* n)
    {
        for (const Endpoint* epSrc : n->get_sources())
        {
            Gate* gSrc = epSrc->get_gate();
            if (!gSrc) continue;

            for (const Endpoint* epDst : n->get_destinations())
            {
                Gate* gDst = epDst->get_gate();
                if (!gDst) continue;

                xmlOut.writeStartElement("edge");
                xmlOut.writeAttribute("source", QString::number(gSrc->get_id()));
                xmlOut.writeAttribute("target", QString::number(gDst->get_id()));
                xmlOut.writeAttribute("id", QString::number(mEdgeId++));
                xmlOut.writeStartElement("attvalues");
                writeEdgeAttribute(xmlOut,n,3);
                writeEdgeAttribute(xmlOut,n,4);
                writeEdgeAttribute(xmlOut,n,5,epSrc->get_pin());
                writeEdgeAttribute(xmlOut,n,6,epDst->get_pin());
                writeEdgeAttribute(xmlOut,n,7);
                xmlOut.writeEndElement();
                xmlOut.writeEndElement();
            }
        }

    }

    void ExportGexf::writeEdgeAttribute(QXmlStreamWriter& xmlOut, const Net* n, int inx, const std::string pin) const
    {
        QString value;
        switch(inx)
        {
        case 3:  value = QString::fromStdString(n->get_name()); break;
        case 4:  value = QString::number(n->get_id()); break;
        case 5:
        case 6:  value = QString::fromStdString(pin); break;
        case 7:  value = "0"; break; // networkx_key
        }

        if (value.isEmpty()) return;

        xmlOut.writeStartElement("attvalue");
        xmlOut.writeAttribute("for", QString::number(inx));
        xmlOut.writeAttribute("value", value);
        xmlOut.writeEndElement();
    }
}
