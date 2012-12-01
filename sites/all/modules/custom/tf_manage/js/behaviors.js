// Using the closure to map jQuery to $.
(function ($) {
  Drupal.behaviors.manage = {
    attach: function (context, settings) {

      /*
      Bind Events
      */

      // Click on a skill or a talent_attribute in the widget
      // sets the nid in the hidden field
      $('.skill.display-widget, .talent_attribute.display-widget').click(function(event) {
        event.stopPropagation();
        console.log('item clicked');

        var nid = get_nid_in_classes($(this).attr('class'));
        $(this).parents('.edit-remark').find('form input[name=attached_to]')
          .val(nid);
        return false;
      });

      // Clicking on none removes the value from the hidden field
      $('.dropdown .none').click(function(event) {
        event.stopPropagation();
        $(this).parents('.edit-remark').find('form input[name=attached_to]')
          .val('');
      });


      /*******
      Helpers
      *******/
      // TODO: Remove duplicate function with tf_recruit
      function get_nid_in_classes(classes) {
        var arr = classes.split(' ');
        var i;
        for(i = 0; i < arr.length; i++) {
          index = arr[i].indexOf("nid-");
          if(index >= 0) {
            return arr[i].substring(index + 4, arr[i].length);
          }
        }
        return false;
      }
      /****
      Main
      ****/
      // Mage remarks in the person details draggable
//      $('.page-manage-person .remark').draggable();


      /**********
        Invokers
      **********/
      // This function is invoked by ajax_command_invoke
      // It finds the unchecked candidates and adds the "unchecked" classs
      $.fn.invoke_hide_remarks = function() {
//        hideRemarks();
      };
    }
  };


  Drupal.behaviors.person_details = {
    attach: function (context, settings) {

    }
  };



  Drupal.behaviors.person_edit = {
    attach: function (context, settings) {
      /*
      Bind Events
      */
      // Make sure when an avatar is changed, the main image changes as well
      $('.person-edit .field-name-field-avatar .image-preview img')
        .load(function(event) {
          event.stopImmediatePropagation();
          var prev_src = $(this).attr('src');
          var larger_src = prev_src.replace('/styles/thumbnail/',
                                                             '/styles/medium/');
          $(this).parents('.person-edit')
            .find('.field.field-name-field-avatar img').attr('src', larger_src);
      });

      // If there is no fid for the avatar, show the default image
      // The default image is stored under default_images/person.jpg
      $('.person-edit input[name=field_avatar_und_0_remove_button]')
        .mousedown(function(event){
          event.stopImmediatePropagation(); // Not sure this is needed
          var current_src = $(this).parents('.person-edit')
                        .find('.field.field-name-field-avatar img').attr('src');
          var current_src_arr = current_src.split('/');
          current_src_arr[current_src_arr.length-1]='default_images/person.jpg';
          var default_src = current_src_arr.join('/');
          // If the current image was the default image we get a double
          // default_images folder. We need to prevent this
          default_src = default_src.replace('/default_images/default_images/',
                                                            '/default_images/');
          $(this).parents('.person-edit')
            .find('.field.field-name-field-avatar img').attr('src',default_src);
      });


      /*******
      Helpers
      *******/
      // Simulate a click on the Add link as a button press
      function simulateAdd(i) {
        return function(event) {
          event.stopImmediatePropagation();
          $('.person-edit input[name="files[field_'
                                        + file_fields[i] + '_und_0]"]').click();
        };
      }

      // Simulate a click on the Remove link of the file field as a button press
      // and add the removed class to the widget
      function simulateRemove(i) {
        return function(event){
          event.stopImmediatePropagation();
          $(this).parents('.' + file_fields[i]).removeClass('defined');
          $(this).parents('.' + file_fields[i]).addClass('removed');
          $('.person-edit input[name=field_'
                        + file_fields[i] + '_und_0_remove_button]').mousedown();
        };
      }

      // Immediately upload when the user has selected an image
      function immediatelyUpload(i) {
        return function(event){
          event.stopImmediatePropagation();
     console.log('immediatelyUpload called');
          if($(this).val() !== '') {
            $('.person-edit #edit-field-'
                         + file_fields[i] + '-und-0-upload-button').mousedown();
          }
          $('.person-edit .' + file_fields[i]).removeClass('removed');
          $('.person-edit .' + file_fields[i]).addClass('defined');
        };
      }

      function simulateFileClick(i) {
        return function(event) {
          event.stopImmediatePropagation();
          $('.person-edit .field-name-field-'+file_fields[i]+' .file-widget a')
                                                                       .click();
        };
      }


      /**********
        Invokers
      **********/
      // This function is invoked by ajax_command_invoke
      // It finds the unchecked candidates and adds the "unchecked" classs
      $.fn.invoke_init_edit_person = function() {
        var i;
        for(i = 0; i < file_fields.length; i++) {
          $('.person-edit .' +file_fields[i]+' .remove').click(simulateRemove(i));
          $('.person-edit .' +file_fields[i]+' .add').click(simulateAdd(i));
          $('.person-edit .' +file_fields[i]+' .file-defined')
                                                     .click(simulateFileClick(i));
          $('.person-edit #edit-field-'
                 + file_fields[i] + '-und-0-upload').change(immediatelyUpload(i));

          // Add classes to the avatar depending if it exists or not
          if($('.person-edit input[name=field_'
                       + file_fields[i] + '_und_0_remove_button]').length === 1) {
            $('.person-edit .' + file_fields[i]).addClass('defined');
          }
          else {
            $('.person-edit .' + file_fields[i]).addClass('removed');
          }
        }

        // Set the default image if no avatar has been set
        $('.person-edit #edit-field-avatar-und-0-upload').once(function() {
          var src = $('.person-edit .avatar .image img').attr('src');
          src = src.replace('/default_images/person.jpg', '');
          $('.person-edit .avatar .image img')
            .attr('src', src + '/default_images/person.jpg');
        });
      };


      /****
      Main
      ****/
      var file_fields = ['avatar', 'cv', 'motivation'];
      $.fn.invoke_init_edit_person();


    }
  };

}(jQuery));
