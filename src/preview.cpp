#include <preview.h>

#include <unity/scopes/ColumnLayout.h>
#include <unity/scopes/PreviewWidget.h>
#include <unity/scopes/PreviewReply.h>
#include <unity/scopes/Result.h>
#include <unity/scopes/VariantBuilder.h>

#include <iostream>

Q_LOGGING_CATEGORY(Prv, "Preview")

namespace sc = unity::scopes;

using namespace std;

Preview::Preview(const sc::Result &result, const sc::ActionMetadata &metadata) :
    sc::PreviewQueryBase(result, metadata) {
}

void Preview::cancelled() {
}

void Preview::run(sc::PreviewReplyProxy const& reply) {

    sc::Result result = PreviewQueryBase::result();

    // Support three different column layouts
    sc::ColumnLayout layout1col(1), layout2col(2), layout3col(3);

    // Single column layout
    layout1col.add_column( { "image_widget", "header_widget", "headline_widget", "exp_widget", "intitle_widget", "instr_widget_0", "instr_widget_1", "instr_widget_2", "buttons_widget" } );

    // Two column layout
    layout2col.add_column( { "image_widget", "intitle_widget", "instr_widget_0", "instr_widget_1", "instr_widget_2" } );
    layout2col.add_column( { "header_widget", "headline_widget", "exp_widget", "buttons_widget" } );

    // Three cokumn layout
    layout3col.add_column( { "image_widget", "buttons_widget" });
    layout3col.add_column( { "header_widget", "headline_widget", "exp_widget" } );
    layout3col.add_column( { "intitle_widget", "instr_widget_0", "instr_widget_1", "instr_widget_2"} );

    // Register the layouts we just created
    reply->register_layout( { layout1col, layout2col, layout3col } );

    // Main visual - video or image.
    if (result.contains("video_url"))
    {
        sc::PreviewWidget image("image_widget", "video");
        image.add_attribute_mapping("source", "video_url");
        image.add_attribute_mapping("screenshot", "art");
        reply->push( { image } );
    }
    else
    {
        sc::PreviewWidget image("image_widget", "image");
        image.add_attribute_mapping("source", "art");
        reply->push( { image } );
    }

    // Define the header section
    sc::PreviewWidget header("header_widget", "header");
    header.add_attribute_mapping("title", "title");

    sc::PreviewWidget headline("headline_widget", "text");
    headline.add_attribute_mapping("text", "headline");

    // Expandable area.
    sc::PreviewWidget exp("exp_widget", "expandable");
    exp.add_attribute_value("title", sc::Variant("Details"));

    sc::PreviewWidget summary("summary_widget", "text");
    summary.add_attribute_mapping("text", "description");

    sc::PreviewWidget departments("departments_widget", "text");
    departments.add_attribute_value("text", sc::Variant("<b>Departments:</b> " + decorate_departments(result["departments"].get_string())));

    sc::PreviewWidget extra("extra_widget", "text");
    extra.add_attribute_value("text", sc::Variant("<b>Extra:</b> " + result["extra"].get_string()));

    sc::PreviewWidget source("source_widget", "text");
    source.add_attribute_value("text", sc::Variant("<b>Source:</b> " + result["source"].get_string()));

    exp.add_widget(summary);
    exp.add_widget(departments);
    exp.add_widget(extra);
    exp.add_widget(source);

    // Define the buttons section
    sc::PreviewWidget buttons("buttons_widget", "actions");
    sc::VariantBuilder builder;
    builder.add_tuple({
                          {"id", sc::Variant("open")},
                          {"label", sc::Variant("Open")},
                          {"uri", result["uri"]}
                      });
//    builder.add_tuple({
//                          {"id", sc::Variant("share")},
//                          {"label", sc::Variant("Share")},
//                          {"uri", result["uri"]}
//                      });
    buttons.add_attribute_value("actions", builder.end());

    reply->push( { header, headline, exp, buttons } );

    // Instructors section.
    if (result.contains("instr_names"))
    {

        sc::PreviewWidget intitle("intitle_widget", "text");
        intitle.add_attribute_value("text", sc::Variant("<b>Instructors:</b>"));
        reply->push( { intitle } );

        sc::VariantArray images = result["instr_images"].get_array();
        sc::VariantArray names = result["instr_names"].get_array();
        sc::VariantArray bios = result["instr_bios"].get_array();

        qCDebug(Prv) << "images" << images.size() << "names" << names.size() << "bios" << bios.size();

        for (uint k = 0; k < names.size(); ++k)
        {
            sc::PreviewWidget comment("instr_widget_" + std::to_string(k), "comment");
            comment.add_attribute_value("source", sc::Variant(images[k].get_string()));
            comment.add_attribute_value("author", sc::Variant(names[k].get_string()));
            comment.add_attribute_value("comment", sc::Variant(bios[k].get_string()));
            reply->push( { comment } );
        }
    }
}

string Preview::decorate_departments(const string &deps)
{
    QString semicolonSeparatedDeps = QString::fromStdString(deps);
    QString decoratedDeps = DepartmentManager::flatDescription(semicolonSeparatedDeps);
    return decoratedDeps.toStdString();
}

