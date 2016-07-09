(function ($) {
  Drupal.behaviors.allstarsTaxonomyGrid = {
    attach: function(context) {
      $('.allstars-taxonomy-grid', context).once('allstars-taxonomy-grid-node-ajax', function() {
        $('.allstars-taxonomy-grid article.node').click(function () {
          var wrapperSelector = Drupal.settings.allstarsTaxonomyGrid.allstarsTaxonomyGridWrapperSelector;
          var wrapper = $(wrapperSelector);
          wrapper.addClass('loader');
          var id = $(this).attr('id');
          var nid = id.split('-').pop();
          var url = '/allstars/taxonomy_grid/node_view/' + nid;
          $.ajax({
            type: "POST",
            url: url,
            success: function(data) {
              wrapper.removeClass('loader');
              wrapper.html(data);
            }
          });
        });
      });
    }
  };
})(jQuery);
