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
          // Get the IDs and set the default type
          var item_id = Drupal.behaviors.tf_common.get_id_in_classes(ui.draggable.attr('class'));
          var to_company_nid = Drupal.behaviors.tf_common.get_id_in_classes($(this).attr('class'));
          var from_company_nid = Drupal.behaviors.tf_common.get_id_in_classes(
                     $(this).parent().find('a.active').parent().attr('class'));
          var item_type = 'node';

          // Get the title: it is different for all types we support
          var item_title = 'item'; // default title


          if(ui.draggable.hasClass('user')) {
            item_type = 'user';
            item_title = ui.draggable.find('.name').html();
          } else if(ui.draggable.hasClass('person')) {
            item_title = ui.draggable.find('.full-name').html();
          } else if(ui.draggable.hasClass('role')) {
            item_title = ui.draggable.find('.field-name-title .field-item').html();
          }

          // Get the from_company and to_company
          var from_company = $('#company-form .form-item-title input').val();
          var to_company = $(this).find('.title').html();

          var msg = '"' + item_title +'" has been moved from "'
                  + from_company + '" to "' + to_company + '"';

          $.ajax({
            url: '/tf_admin/move/' + item_type
                             + '/' + item_id
                             + '/' + from_company_nid
                             + '/' + to_company_nid
          });

          Drupal.behaviors.tf_common.set_message(msg, 'status');
          ui.draggable.soft_remove();
        },
        tolerance: 'pointer',
        hoverClass: 'dnd-hover',
        activeClass: 'dnd-active'
      });

    }
  };
}(jQuery));
