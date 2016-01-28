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
    layout1col.add_column( { "image_widget", "header_widget", "summary_widget", "buttons_widget" } );

    // Two column layout
    layout2col.add_column( { "image_widget", "buttons_widget" } );
    layout2col.add_column( { "header_widget", "summary_widget" } );

    // Three cokumn layout
    layout3col.add_column( { "image_widget", "buttons_widget" });
    layout3col.add_column( { "header_widget", "summary_widget" } );
    layout3col.add_column( { } );

    // Register the layouts we just created
    reply->register_layout( { layout1col, layout2col, layout3col } );

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
    // header.add_attribute_mapping("subtitle", "subtitle");

    sc::PreviewWidget summary("summary_widget", "text");
    summary.add_attribute_mapping("text", "description");

    // Define the buttons section
    sc::PreviewWidget buttons("buttons_widget", "actions");
    sc::VariantBuilder builder;
    builder.add_tuple({
                          {"id", sc::Variant("open")},
                          {"label", sc::Variant("Open")},
                          {"uri", result["uri"]}
                      });
    buttons.add_attribute_value("actions", builder.end());

    reply->push( { header, summary, buttons } );
}

