sealed trait IntList
case object Nil extends IntList
case class Cons(v: Int, t: IntList) extends IntList

sealed trait BTree
case object Leaf extends BTree
case class IntNode(v: Int, left: BTree, right: BTree) extends BTree

sealed trait Formula
case object True extends Formula
case object False extends Formula
case class Not(f: Formula) extends Formula
case class Andalso(left: Formula, right: Formula) extends Formula
case class Orelse(left: Formula, right: Formula)  extends Formula
case class Implies(left: Formula, right: Formula) extends Formula

object Hw1 extends App {

  println("Hw1!")

  def gcd(a: Int, b: Int): Int = {
    if(a%b==0)
      b
    else{
      val r = a%b
      gcd(b, r)
    }
  }

  def oddSum(f: Int=>Int, n: Int): Int = {
    if(n==1)
      f(1)
    else{
      if(n%2==0)
        oddSum(f,n-1)
      else
        f(n) + oddSum(f, n-2)
    }
  }

  def foldRight(init: Int, ftn: (Int, Int)=>Int, list: IntList): Int = {
    list match{
      case Nil => init
      case Cons(h, t) => ftn(foldRight(init, ftn, t), h)
    }
  }

  def map(f: Int=>Int, list: IntList): IntList = {
    list match{
      case Nil => list
      case Cons(h, t) => Cons(f(h), map(f,t))
    }
  }

  def iter[A](f: A => A, n: Int): A => A = (x: A) => {
    if(n==0)
      x
    else{
      iter(f, n-1)(f(x))
    }
  }  
  
  def insert(t: BTree, a: Int): BTree = {
    t match{
      case Leaf => IntNode(a, Leaf, Leaf)
      case IntNode(v, left, right)=>{
        if(a<v){
          IntNode(v, insert(left, a), right)
        }
        else{
          IntNode(v, left, insert(right, a))
        }
      }
    }
  }

  def eval(f: Formula): Boolean = {
    f match{
      case True => true
      case False => false
      case Not(i) => {
        if(i==True)
          eval(False)
        else
          eval(True)
      }
      case Andalso(l, r) => {
        if(eval(l)==true && eval(r)==true)
          eval(True)
        else
          eval(False)
      }
      case Orelse(l, r) => {
        if(eval(l)==true || eval(r)==true)
          eval(True)
        else
          eval(False)
      }
      case Implies(l, r) => {
        if(eval(l)==true && eval(r)==false)
          eval(False)
        else
          eval(True)
      }
    }
  }

}