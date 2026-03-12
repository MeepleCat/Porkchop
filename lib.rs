use wasm_bindgen::prelude::*;
use web_sys::{window, HtmlInputElement};
use wasm_bindgen::closure::Closure;
use wasm_bindgen::JsCast;

#[wasm_bindgen(start)]
pub fn start() {
    let doc = window().unwrap().document().unwrap();

    let button = doc.get_element_by_id("test").unwrap();
    let closure = Closure::wrap(Box::new(move || {
        let doc = window().unwrap().document().unwrap();
        let input: HtmlInputElement = doc.get_element_by_id("itest").unwrap().dyn_into().unwrap();

        let a : f64 = input.value().parse().unwrap_or(0.0);

        doc.get_element_by_id("test").unwrap().set_inner_html(&format!("{}", a));
    }) as Box<dyn Fn()>);

    button.add_event_listener_with_callback("click", closure.as_ref().unchecked_ref()).unwrap();
    closure.forget();
}