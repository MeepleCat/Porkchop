use wasm_bindgen::prelude::*;
use wasm_bindgen::closure::Closure;
use wasm_bindgen::JsCast;
use web_sys::{window, HtmlInputElement};

#[wasm_bindgen(start)]
pub fn start() {
    let document = window().unwrap().document().unwrap();

    // Get the button element
    let button = document.get_element_by_id("test").unwrap();

    // Clone it so it can be moved into the closure
    let button_clone = button.clone();

    // Create click handler
    let closure = Closure::wrap(Box::new(move || {
        let document = window().unwrap().document().unwrap();

        // Get the input element
        let input: HtmlInputElement = document
            .get_element_by_id("itest")
            .unwrap()
            .dyn_into()
            .unwrap();

        // Read and parse value
        let value_str = input.value();
        let parsed: f64 = value_str.trim().parse().unwrap_or(0.0);

        // Update button text
        button_clone.set_text_content(Some(&format!("{}", parsed)));
    }) as Box<dyn Fn()>);

    // Attach listener
    button
        .add_event_listener_with_callback("click", closure.as_ref().unchecked_ref())
        .unwrap();

    // Prevent closure from being dropped
    closure.forget();
}