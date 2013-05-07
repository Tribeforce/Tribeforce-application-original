// Copyright (c) 2013 Femi Veys


// Using the closure to map jQuery to $.
(function ($) {
  Drupal.behaviors.feedback = {
    attach: function (context, settings) {
      var o = 'once-feedback';

      /*
      Bind Events
      */

      // Click on a skill or a talent_attribute in the widget
      // sets the nid in the hidden field
      $('.dropdown > div.display-widget').once(o).click(function(event) {
        var nid = Drupal.behaviors.tf_common.get_id_in_classes($(this).attr('class'));
        $(this).parents('.edit-remark').find('form input[name=attached_to]')
          .val(nid);
      });

      // Clicking on NONE removes the value from the hidden field
      $('.dropdown .none').once(o).click(function(event) {
        $(this).parents('.edit-remark').find('form input[name=attached_to]')
          .val('');
      });

      /****
      Main
      ****/
      // Make remarks in the person details draggable
//      $('.page-feedback-person .remark').draggable();


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


      /****
      Main
      ****/
      var file_fields = ['avatar', 'cv', 'motivation'];


      /*
      Bind Events
      */
      // Make sure when an avatar is changed, the main image changes as well
      $('.person-edit .field-name-field-avatar .image-preview img')
        .load(function(event) {
          event.stopImmediatePropagation();
          var src = $(this).attr('src');
            $(this).parents('.person-edit')
            .find('.field.field-name-field-avatar img').attr('src', src);
      });

      // If there is no fid for the avatar, show the default image
      // The default image is stored under default_images/person.jpg
      $('.person-edit input[name=field_avatar_und_0_remove_button]')
        .mousedown(function(event){
          event.stopImmediatePropagation(); // Not sure this is needed
          var default_src = '/sites/default/files/person.jpg';
          $(this).parents('.person-edit')
            .find('.field.field-name-field-avatar img').attr('src',default_src);
      });


      // When the file upload fails, we receive an error messages
      // > We move the message to our message area
      // > We make sure the the upload field show we did not upload anything
      $('.field-type-file .error').each(function() {
        var prefix = 'field-name-field-';
        var type = '';
        var i;

        $origin = $(this).parents('div[class*="' + prefix + '"]');
        for(i = 0; i < file_fields.length; i++) {
          if($origin.hasClass(prefix+file_fields[i])) {
            type = file_fields[i];
            break;
          }
        }
        $('.person-edit .' + type).removeClass('defined');
        $('.person-edit .' + type).addClass('removed');

        $('.tf-region.messages .error').remove();
        $('.tf-region.messages').append($(this));
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

      // Simulate a click on the image as a remove followed by a add.
      // TODO: This is code duplication. Reuse simulateRemove and simulateAdd
      function simulateEdit(i) {
        return function(event) {
//          event.stopImmediatePropagation();
//          event.preventDefault();

          // Remove code
          $(this).parents('.' + file_fields[i]).removeClass('defined');
          $(this).parents('.' + file_fields[i]).addClass('removed');
          $('.person-edit input[name=field_'
                      + file_fields[i] + '_und_0_remove_button]').mousedown();

          // Add code
//          setTimeout(function(i) {
//            console.log($('.person-edit input[name="files[field_'
//                                      + file_fields[i] + '_und_0]"]').length);

//            $('.person-edit input[name="files[field_'
//                                      + file_fields[i] + '_und_0]"]').click();
//          }, 5000);

          clickUpload(i);

        };

      }

      function clickUpload(i) {
        setTimeout(function() {
//          var selector = ;
//          if($(selector).length > 0) {
//            console.log($selector);
  console.log('.person-edit input[name="files[field_'
                              + file_fields[i] + '_und_0]"]');
  console.log($('.person-edit input[name="files[field_'
                              + file_fields[i] + '_und_0]"]').length);
            $('.person-edit input[name="files[field_'
                              + file_fields[i] + '_und_0]"]').click(); // No idea why this doesn't always work
//            return true;
//          }
//          else {
//          }
        }, 900);
      }


      // Immediately upload when the user has selected an image
      function immediatelyUpload(i) {
        return function(event){
          event.stopImmediatePropagation();
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
          $('.person-edit .' +file_fields[i]+' .edit').click(simulateEdit(i));
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
//        $('.person-edit #edit-field-avatar-und-0-upload').once(function() {
//          var src = $('.person-edit .avatar .image img').attr('src');
//          src = src.replace('/default_images/person.jpg', '');
//          $('.person-edit .avatar .image img')
//            .attr('src', src + '/default_images/person.jpg');
//        });
      };

      $.fn.invoke_init_edit_person();



    }
  };

}(jQuery));
