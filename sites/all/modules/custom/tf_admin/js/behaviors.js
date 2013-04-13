// Using the closure to map jQuery to $.
(function ($) {
  Drupal.behaviors.admin = {
    attach: function (context, settings) {

      $('.tf-module.admin .person, .tf-module.admin .role, .tf-module.admin .user').draggable({
        cursor: 'move',
        opacity: 0.5,
        revert: "invalid"
      });


      $('.tf-module.admin .companies .item').droppable({
        drop: function( event, ui ) {
          var item_id = Drupal.behaviors.tf_common.get_id_in_classes(ui.draggable.attr('class'));
          var to_company_nid = Drupal.behaviors.tf_common.get_id_in_classes($(this).attr('class'));
          var from_company_nid = Drupal.behaviors.tf_common.get_id_in_classes(
                     $(this).parent().find('a.active').parent().attr('class'));

          $.ajax({
            url: '/tf_admin/move/' + item_id
                             + '/' + from_company_nid
                             + '/' + to_company_nid
          });

          ui.draggable.soft_remove();

        },
        tolerance: 'pointer',
        hoverClass: 'dnd-hover',
        activeClass: 'dnd-active'
      });

    }
  };
}(jQuery));
